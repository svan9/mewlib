#ifndef MEW_COLORS_LIB_SO2U
#define MEW_COLORS_LIB_SO2U

namespace mew::colors {
	constexpr const char* reset = "\033[0m";
	constexpr const char* red = "\033[31m";
	constexpr const char* green = "\033[32m";
	constexpr const char* yellow = "\033[33m";
	constexpr const char* blue = "\033[34m";
	constexpr const char* magenta = "\033[35m";
	constexpr const char* cyan = "\033[36m";
	constexpr const char* white = "\033[37m";
	constexpr const char* bright_red = "\033[91m";
	constexpr const char* bright_green = "\033[92m";
	constexpr const char* bright_yellow = "\033[93m";
	constexpr const char* bright_blue = "\033[94m";
	constexpr const char* bright_magenta = "\033[95m";
	constexpr const char* bright_cyan = "\033[96m";
	constexpr const char* bright_white = "\033[97m";
	constexpr const char* bg_red = "\033[41m";
	constexpr const char* bg_green = "\033[42m";
	constexpr const char* bg_yellow = "\033[43m";
	constexpr const char* bg_blue = "\033[44m";
	constexpr const char* bg_magenta = "\033[45m";
	constexpr const char* bg_cyan = "\033[46m";
	constexpr const char* bg_white = "\033[47m";
	constexpr const char* bright_bg_red = "\033[101m";
	constexpr const char* bright_bg_green = "\033[102m";
	constexpr const char* bright_bg_yellow = "\033[103m";
	constexpr const char* bright_bg_blue = "\033[104m";
	constexpr const char* bright_bg_magenta = "\033[105m";
	constexpr const char* bright_bg_cyan = "\033[106m";
	constexpr const char* bright_bg_white = "\033[107m";
	constexpr const char* bold = "\033[1m";
	constexpr const char* underline = "\033[4m";
	constexpr const char* blink = "\033[5m";
	constexpr const char* reverse = "\033[7m";
	constexpr const char* hidden = "\033[8m";
	constexpr const char* strikethrough = "\033[9m";
	constexpr const char* bright = "\033[90m";
	constexpr const char* bright_red_bg = "\033[100m";
	constexpr const char* bright_green_bg = "\033[102m";
	constexpr const char* bright_yellow_bg = "\033[103m";
	constexpr const char* bright_blue_bg = "\033[104m";
	constexpr const char* bright_magenta_bg = "\033[105m";
	constexpr const char* bright_cyan_bg = "\033[106m";
	constexpr const char* bright_white_bg = "\033[107m";
	constexpr const char* bright_black = "\033[90m";
	constexpr const char* bright_red_fg = "\033[91m";
	constexpr const char* bright_green_fg = "\033[92m";
	constexpr const char* bright_yellow_fg = "\033[93m";
	constexpr const char* bright_blue_fg = "\033[94m";
	constexpr const char* bright_magenta_fg = "\033[95m";
	constexpr const char* bright_cyan_fg = "\033[96m";
	constexpr const char* bright_white_fg = "\033[97m";
	constexpr const char* bright_black_fg = "\033[90m";
	constexpr const char* bright_black_bg = "\033[100m";
	constexpr const char* bright_red_bg = "\033[101m";
	constexpr const char* bright_green_bg = "\033[102m";
	constexpr const char* bright_yellow_bg = "\033[103m";
	constexpr const char* bright_blue_bg = "\033[104m";
	constexpr const char* bright_magenta_bg = "\033[105m";
	constexpr const char* bright_cyan_bg = "\033[106m";
	constexpr const char* bright_white_bg = "\033[107m";
	
	#define RED(text) (mew::colors::red text mew::colors::reset)
	#define GREEN(text) (mew::colors::green text mew::colors::reset)
	#define YELLOW(text) (mew::colors::yellow text mew::colors::reset)
	#define BLUE(text) (mew::colors::blue text mew::colors::reset)
	#define MAGENTA(text) (mew::colors::magenta text mew::colors::reset)
	#define CYAN(text) (mew::colors::cyan text mew::colors::reset)
	#define WHITE(text) (mew::colors::white text mew::colors::reset)
	#define BRIGHT_RED(text) (mew::colors::bright_red text mew::colors::reset)
	#define BRIGHT_GREEN(text) (mew::colors::bright_green text mew::colors::reset)
	#define BRIGHT_YELLOW(text) (mew::colors::bright_yellow text mew::colors::reset)
	#define BRIGHT_BLUE(text) (mew::colors::bright_blue text mew::colors::reset)
	#define BRIGHT_MAGENTA(text) (mew::colors::bright_magenta text mew::colors::reset)
	#define BRIGHT_CYAN(text) (mew::colors::bright_cyan text mew::colors::reset)
	#define BRIGHT_WHITE(text) (mew::colors::bright_white text mew::colors::reset)
	#define BG_RED(text) (mew::colors::bg_red text mew::colors::reset)
	#define BG_GREEN(text) (mew::colors::bg_green text mew::colors::reset)
	#define BG_YELLOW(text) (mew::colors::bg_yellow text mew::colors::reset)
	#define BG_BLUE(text) (mew::colors::bg_blue text mew::colors::reset)
	#define BG_MAGENTA(text) (mew::colors::bg_magenta text mew::colors::reset)
	#define BG_CYAN(text) (mew::colors::bg_cyan text mew::colors::reset)
	#define BG_WHITE(text) (mew::colors::bg_white text mew::colors::reset)
	#define BRIGHT_BG_RED(text) (mew::colors::bright_bg_red text mew::colors::reset)
	#define BRIGHT_BG_GREEN(text) (mew::colors::bright_bg_green text mew::colors::reset)
	#define BRIGHT_BG_YELLOW(text) (mew::colors::bright_bg_yellow text mew::colors::reset)
	#define BRIGHT_BG_BLUE(text) (mew::colors::bright_bg_blue text mew::colors::reset)
	#define BRIGHT_BG_MAGENTA(text) (mew::colors::bright_bg_magenta text mew::colors::reset)
	#define BRIGHT_BG_CYAN(text) (mew::colors::bright_bg_cyan text mew::colors::reset)
	#define BRIGHT_BG_WHITE(text) (mew::colors::bright_bg_white text mew::colors::reset)
	#define BOLD(text) (mew::colors::bold text mew::colors::reset)
	#define UNDERLINE(text) (mew::colors::underline text mew::colors::reset)
	#define BLINK(text) (mew::colors::blink text mew::colors::reset)
	#define REVERSE(text) (mew::colors::reverse text mew::colors::reset)
	#define HIDDEN(text) (mew::colors::hidden text mew::colors::reset)
	#define STRIKETHROUGH(text) (mew::colors::strikethrough text mew::colors::reset)
	#define BRIGHT(text) (mew::colors::bright text mew::colors::reset)
	#define BRIGHT_BLACK(text) (mew::colors::bright_black text mew::colors::reset)
	#define BRIGHT_RED_FG(text) (mew::colors::bright_red_fg text mew::colors::reset)
	#define BRIGHT_GREEN_FG(text) (mew::colors::bright_green_fg text mew::colors::reset)
	#define BRIGHT_YELLOW_FG(text) (mew::colors::bright_yellow_fg text mew::colors::reset)
	#define BRIGHT_BLUE_FG(text) (mew::colors::bright_blue_fg text mew::colors::reset)
	#define BRIGHT_MAGENTA_FG(text) (mew::colors::bright_magenta_fg text mew::colors::reset)
	#define BRIGHT_CYAN_FG(text) (mew::colors::bright_cyan_fg text mew::colors::reset)
	#define BRIGHT_WHITE_FG(text) (mew::colors::bright_white_fg text mew::colors::reset)
	#define BRIGHT_BLACK_FG(text) (mew::colors::bright_black_fg text mew::colors::reset)
	#define BRIGHT_BLACK_BG(text) (mew::colors::bright_black_bg text mew::colors::reset)
	#define BRIGHT_RED_BG(text) (mew::colors::bright_red_bg text mew::colors::reset)
	#define BRIGHT_GREEN_BG(text) (mew::colors::bright_green_bg text mew::colors::reset)
	#define BRIGHT_YELLOW_BG(text) (mew::colors::bright_yellow_bg text mew::colors::reset)
	#define BRIGHT_BLUE_BG(text) (mew::colors::bright_blue_bg text mew::colors::reset)
	#define BRIGHT_MAGENTA_BG(text) (mew::colors::bright_magenta_bg text mew::colors::reset)
	#define BRIGHT_CYAN_BG(text) (mew::colors::bright_cyan_bg text mew::colors::reset)
	#define BRIGHT_WHITE_BG(text) (mew::colors::bright_white_bg text mew::colors::reset)


	
}

#endif