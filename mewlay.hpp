#ifndef SO2U_MEWLAY
#define SO2U_MEWLAY

#ifndef __cplusplus
#error cpp defined lib
#endif

#include "mewtypes.h"
#include "mewstack.hpp"
#include <variant>

#define LAYOUT(opt)                                                                                             \
	for (                                                                                                         \
			::MewLay::nanlay_layout_element_queue = (::MewLay::OpenElement(), ::MewLay::WrapElement(opt), 0); \
			::MewLay::nanlay_layout_element_queue < 1;                                                                \
			++::MewLay::nanlay_layout_element_queue, ::MewLay::CloseElement())

namespace MewLay {
	typedef u32(*GetU32)(void);
	typedef void(*Draw4Line)(u32 a, u32 b, u32 c, u32 d, u32 color);
	typedef void(*Draw4Circle)(u32 x, u32 y, float radius, u32 color);
	typedef void(*Draw4Arc)(u32 x, u32 y, float radius, float start4radius, float end4radius, u32 color);

	static GetU32 mewlay__GetWindowWidth = nullptr;
	static GetU32 mewlay__GetWindowHeight = nullptr;
	static Draw4Line mewlay__DrawLine = nullptr;
	static Draw4Circle mewlay__DrawCircle = nullptr;
	static Draw4Arc mewlay__DrawArc = nullptr;

	
	
	struct Unit {
		enum struct Type: u8 {
			Percentage, Pixels
		} type;

		u8 percentage;
		u32 pixels = -1;

		static Unit byPixels(u32 unit) {
			return Unit{.type = Type::Pixels, .pixels = unit};
		}
		static Unit byPercentage(u8 unit) {
			return Unit{.type = Type::Percentage, .percentage = unit};
		}
		
		void forceUnit(u32 unit) {
			switch (type) {
				case Type::Percentage: {
					double abs_pc = this->percentage / 100;
					this->pixels = (u32)(abs_pc * unit);
				} break;
				case Type::Pixels: {
					double abs_pc = this->pixels / unit;
					this->percentage = (u8)(abs_pc * 100);
				} break;
			}
		}

		bool hasValue() {
			return pixels != (u32)(-1);
		}

		u32 getValue() {
			return pixels;
		}

		Unit& operator=(u32 unit) {
			type = Type::Pixels;
			pixels = unit;
		}
	};

	struct Color {
		union {
			struct { u8 r, g, b, a; };
			struct { u32 rgba; };
		};
		Color() {}
		Color(u32 rgba) { this->rgba = rgba; }
		Color(u8 r, u8 g, u8 b, u8 a = 255) 
			{ this->r = r; this->g = g; this->b = b; this->a = a; }
	};

	template<typename UnitType>
	struct SideUnits { 
		UnitType top;
		UnitType left;
		UnitType right;
		UnitType bottom;

		SideUnits() {}
		SideUnits(UnitType unit): top(unit), left(unit), right(unit), bottom(unit) {}
		SideUnits(UnitType top_bot, UnitType left_right): top(top_bot), left(left_right), right(left_right), bottom(top_bot) {}
		SideUnits(UnitType top, UnitType left, UnitType right, UnitType bottom): top(top), left(left), right(right), bottom(bottom) {}
	};
	
	struct Border {
		SideUnits<::MewLay::Color> color;
		SideUnits<Unit> width;
		SideUnits<Unit> radius;

		Border() {}

		void forceUnits(u32 width, u32 height) {

		}
	};

	struct Background {
		::MewLay::Color color;
	};

	struct Style {
		const char* text = nullptr;
		Border border;
		Background background;
		Unit width;
		Unit height;
		Unit x;
		Unit y;

		void forceUnits(Style& parent_style) {
			if (parent_style.width.hasValue()) {
				width.forceUnit(parent_style.width.getValue());
			}
			if (parent_style.height.hasValue()) {
				height.forceUnit(parent_style.height.getValue());
			}
			if (parent_style.x.hasValue()) {
				x.forceUnit(parent_style.x.getValue());
			}
			if (parent_style.y.hasValue()) {
				y.forceUnit(parent_style.y.getValue());
			}
		}
	};
	
	struct Element {
		mew::stack<Element*> children;
		Element* parent;
		Style style;
		u64 id;

		Element() {}
		
		static Element* getRoot(void) {
			static Element* __root = new Element(); 
			if (mewlay__GetWindowWidth != nullptr && mewlay__GetWindowHeight != nullptr) {
				__root->style.width = mewlay__GetWindowWidth();
				__root->style.height = mewlay__GetWindowHeight();
			} else {
				__root->style.width = 0;
				__root->style.height = 0;
			}
			__root->style.x = 0;
			__root->style.y = 0;
			return __root;
		}
	};

	struct LineCommand {
		u32 x, y, x2, y2;
		u32 color;

		LineCommand() {}
		LineCommand(u32 x, u32 y, u32 x2, u32 y2, u32 color)
			: x(x), y(y), x2(x2), y2(y2), color(color) {}
	};

	struct CircleCommand {
		u32 x, y;
		float radius;
		u32 color;
	};
	
	struct ArcCommand {
		u32 x, y;
		float radius, start, end;
		u32 color;

		ArcCommand() {}
		ArcCommand(u32 x, u32 y, float radius, float start, float end, u32 color)
			: x(x), y(y), radius(radius), start(start), end(end), color(color) {}
	};
	
	using Command = std::variant<LineCommand, CircleCommand, ArcCommand>;

	struct Context {
		mew::stack<Element> elements;
		mew::stack<Command> commands;
		
		static Context* getCurrent(void) {
			static Context* nanlay_currentContext = new Context();
			return nanlay_currentContext;
		}
	};
	
	struct WrapOptions {
		Style style;
	};
		
	static u32 nanlay_layout_element_queue;
	
	void OpenElement(void) { 
		Context* ctx = Context::getCurrent();
		Element* parent;
		if (!ctx->elements.empty()) {
			parent = &ctx->elements.at(-1);
		} else {
			parent = Element::getRoot();
		}
		ctx->elements.push(Element{}); 
		Element* current = &ctx->elements.at(-1);
		current->parent = parent;
		parent->children.push(current);
	}

	void WrapElement(WrapOptions opt) {
		Context* ctx = Context::getCurrent();
		auto& current = ctx->elements.at(-1);
		current.style = opt.style;
		auto& parent = current.parent;
		if (parent) {
			current.style.forceUnits(parent->style);
		}
	}
	
	void CloseElement(void) {
		Context* ctx = Context::getCurrent();		
		ctx->elements.pop();
	}
	
	void DrawRectangle4Corner(u32 x, u32 y, u32 width, u32 height, u32 left_rad, u32 right_rad, u32 top_rad, u32 bot_rad, ::MewLay::Color color) {
		Context* ctx = Context::getCurrent();		
		u32 max_x = x+width;
		u32 max_y = y+height;
		u32 _color = color.rgba;
		
		// top side
		ctx->commands.push(LineCommand(x + left_rad, y, max_x-right_rad, y, _color));	
		// right side
		ctx->commands.push(LineCommand(max_x, y+top_rad, max_x, max_y-bot_rad, _color));	
		// bottom side
		ctx->commands.push(LineCommand(x + left_rad, max_y, max_x-right_rad, max_y, _color));	
		// left side
		ctx->commands.push(LineCommand(x, y+top_rad, x, max_y-bot_rad, _color));	

		// top-right corner
		ctx->commands.push(ArcCommand(max_x - right_rad, y + top_rad, right_rad, mew::math::pi * 1.5, 0, _color));
		// bottom-right corner
		ctx->commands.push(ArcCommand(max_x - right_rad, max_y - bot_rad, right_rad, 0, mew::math::pi / 2, _color));
		// bottom-left corner
		ctx->commands.push(ArcCommand(x + left_rad, max_y - top_rad, left_rad, mew::math::pi / 2, mew::math::pi, _color));
		// top-left corner
		ctx->commands.push(ArcCommand(x + left_rad, y + top_rad, left_rad, mew::math::pi, mew::math::pi * 1.5, _color));
	}

	void DrawRectangle4Corner(u32 x, u32 y, u32 width, u32 height, u32 radius, ::MewLay::Color color) {
		DrawRectangle4Corner(x, y, width, height, radius, radius, radius, radius, color);
	}

	void DrawCommand(Command& command) {
		std::visit(
			[](auto&& command) {
				if constexpr (mew::same_as<decltype(command), LineCommand>) {
					LineCommand& cmd = (LineCommand)command;
					mewlay__DrawLine(cmd.x, cmd.y, cmd.x2, cmd.y2, cmd.color);
				}
				else if constexpr (mew::same_as<decltype(command), CircleCommand>) {
					CircleCommand& cmd = (CircleCommand)command;
					mewlay__DrawCircle(cmd.x, cmd.y, cmd.radius, cmd.color);
				}
				else if constexpr (mew::same_as<decltype(command), ArcCommand>) {
					ArcCommand& cmd = (ArcCommand)command;
					mewlay__DrawArc(cmd.x, cmd.y, cmd.radius, cmd.start, cmd.end);
				}
			},
		 command);
	}
	
	void Display(void) {
		MewUserAssert(mewlay__DrawLine && mewlay__DrawCircle && mewlay__DrawArc, "can't drawing");
		Context* ctx = Context::getCurrent();		
		
		for (u32 i = 0; ctx->commands.count(); ++i) {
			auto command = ctx->commands[i];
			DrawCommand(command);
		}
	}
	
	void BeginDrawingLayouts(void) {
		Context* ctx = Context::getCurrent();		
		ctx->commands.clear();
		ctx->elements.clear();
	}

	void EndDrawingLayouts(void) {
		Display();
	}
}

namespace Tests {
	void MewLay__example(void) {
		LAYOUT(MewLay::WrapOptions{.style={
			
		}}) {
			
		}
	}
}

#ifdef RAYLIB_H
	namespace MewLay::RayLib {
		void DrawLine4Mewlay(u32 a, u32 b, u32 c, u32 d, u32 color) {
			::DrawLine(a, b, c, d, (::Color)color);
		}

		void DrawCircle4Mewlay(u32 x, u32 y, float radius, u32 color) {
			::DrawCircle(x, y, radius, (::Color)color);
		}

		void DrawArc4Mewlay(u32 x, u32 y, float radius, float start4radius, float end4radius, u32 color) {
			::DrawCircleSector(::Vector2{(float)x, (float)y}, radius, start4radius, end4radius, 36, (::Color)color);
		}

		u32 GetWindowWidth4MewLay(void) {
			return ::GetScreenWidth();
		}
		u32 GetWindowHeight4MewLay(void) {
			return ::GetScreenHeight();
		}
		
	};
	MewLay::Draw4Line MewLay::mewlay__DrawLine = ::MewLay::RayLib::DrawLine4Mewlay;
	MewLay::Draw4Circle MewLay::mewlay__DrawCircle = ::MewLay::RayLib::DrawCircle4Mewlay;
	MewLay::Draw4Arc MewLay::mewlay__DrawArc = ::MewLay::RayLib::DrawArc4Mewlay;
	MewLay::GetU32 MewLay::mewlay__GetWindowWidth = ::MewLay::RayLib::GetWindowWidth4MewLay;
	MewLay::GetU32 MewLay::mewlay__GetWindowHeight = ::MewLay::RayLib::GetWindowHeight4MewLay;

#endif
#endif