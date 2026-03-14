#ifndef MEW_STATE_HPP
#define MEW_STATE_HPP

#include "mewtypes.h"
#include "mewstack"
#include <typeinfo>

namespace mew {
	namespace Internal::State {
		class StateManager;
		
		using type_info = std::type_info;
		template<typename T>
		using Stack = ::mew::stack<T>;
		template<typename T>
		using SetState = void(*)(T&& value);
		template<typename T>
		using StateEffect = void(*)(const T& value);

		struct State {
			byte* data = nullptr;
			type_info type;
			StateManager* state_manager;
			Stack<void*> effects;
		};

		template<typename T>
		struct StateTuple {
			const T& state;
			SetState<T> setter;

			~StateTuple() {
				StateManager::getCurrent()->_popState();
			}
		};

		class StateManager {
		private:
			Stack<State> states;
			u64 counter = 0;
		public:
			StateManager() {}

			static StateManager* getCurrent(void) {
				static StateManager* current = new StateManager();
				return current;
			}
			
			template<typename T>
			void _pushState(const State& state) {
				_pushState<T>(state, counter++);
			}

			template<typename T>
			void _pushState(const State& state, u64 idx) {
				if (states.has(idx)) {
					MewUserAssert(states.at(idx).type == state.type, "state type mismatch");
					states.at(idx) = state;
				} else {
					states.push(state);
				}
				if (states.at(idx).effects.count() > 0) {
					for (u64 i = 0; i < states.at(idx).effects.count(); ++i) {
						void* raw_effect = states.at(idx).effects[i];
						StateEffect<T> effect = (StateEffect<T>)raw_effect;
						const T& value = *(T*)states.at(idx).data;
						effect(value);
					}
				}
			}

			// template<typename T>
			// void _addEffect(StateEffect<T> effect, ) {
			
			void _resetCounter() { 
				counter = 0;
			}

			void _popState(void) {
				if (states.has(counter - 1)) {
					--counter;
				}
			}

			template<typename T>
			const T& _getState() {
				MewUserAssert(states.has(counter - 1), "state not found");
				MewUserAssert(states.at(counter - 1).type == typeid(T), "state type mismatch");
				return *(T*)states.at(counter - 1).data;
			}

			template<typename T>
			static const T& getState() {
				return getCurrent()->_getState<T>();
			}

			static void resetCounter(void) {
				getCurrent()->_resetCounter();
			}

			static void popState(void) {
				getCurrent()->_popState();
			}

						
			template<typename T>
			static void pushState(const State& state) {
				getCurrent()->_pushState<T>(state);
			}			
			
		};
	}

	template<typename T>
	const Internal::State::StateTuple<T>
		useState(T&& initial) {
		using namespace Internal::State;
		StateManager::pushState<T>(State{
			.date = (byte*)new T(initial),
			.type = typeid(T),
			.state_manager = StateManager::getCurrent()
		});
		return StateTuple<T>{
			.state = *(T*)StateManager::getCurrent()->states.at(StateManager::getCurrent()->counter - 1).date,
			.setter = [](T&& value) {
				StateManager::getCurrent()->states.at(StateManager::getCurrent()->counter - 1).date = (byte*)new T(value);
			}
		};
	}

	
	template<typename T>
	const Internal::State::StateTuple<T>
		useState() { 
		using namespace Internal::State;
		StateManager::pushState<T>(State{
			.date = (byte*)new T(),
			.type = typeid(T),
			.state_manager = StateManager::getCurrent()
		});
		return StateTuple<T>{
			.state = *(T*)StateManager::getCurrent()->states.at(StateManager::getCurrent()->counter - 1).date,
			.setter = [](T&& value) {
				StateManager::getCurrent()->states.at(StateManager::getCurrent()->counter - 1).date = (byte*)new T(value);
			}
		}
	}
	
	namespace Tests {
		void testStateComponent1(void) {
			auto [state, setState] = useState<int>(0);
			MewTest(state, 0);
			for (int i = 0; i < 10; ++i) {
				mew::sleep_ms(100);
				setState(1+state);
				MewTest(state, 1+i);
			}
		}
		void testState(void) {
			auto [state1, setState1] = useState<int>(0);
			auto [state2, setState2] = useState<float>(0.0f);
			
			MewTest(state1, 0);
			MewTest(state2, 0.0f);

			setState1(10);
			setState2(20.0f);

			MewTest(state1, 10);
			MewTest(state2, 20.0f);
			testStateComponent1();
		}
	}
	
	
};


#endif