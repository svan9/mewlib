#ifndef MEW_COMPILER_ADDITION
#define MEW_COMPILER_ADDITION

#include "mewlib.h"
#include "mewstack.hpp"
#include "mewutils.hpp"
#include <unordered_map>

#pragma pack(push, 1)

namespace mew::cad {

	template<typename TokenType>
	class Compiler {
	public:
		typedef std::unordered_map<const char *, TokenType> token_table_t;
		token_table_t tokens;

		struct Flags {
			bool use_string: 1 = true;
		} flags;

		Compiler(token_table_t tokens): tokens(tokens) {}

		namespace Contexts {
			struct DefaultTokenContext {
				size_t line_idx;
				DefaultTokenContext() {}
				DefaultTokenContext(size_t line): line_idx(line) {}
			};

			struct Number: DefaultTokenContext {
				size_t size;
				Number(size_t size): size(size) {}
			};
		};
	
		struct Token {
			TokenType type;
			const char* value = nullptr;
			void* context;
		};
		
		typedef mew::stack<Token> tokens_t;
		typedef mew::stack<tokens_t> token_row_t;
		typedef bool(*token_watcher)(Token& tk);

		struct Lexer {
			token_row_t token_row;
			mew::stack<const char*> lines;
			const char* input_file = "local";
			const bool use_beauty_error;
			token_watcher watcher = 0;

			void _fill_simple() {
				for (int i = 0; i < lines.size(); ++i) {
					mew::stack<Token> token_line;
					const char* line = lines[i];
					utils::TokenRow str_row(line);
					const char* word;
					bool is_comment = false;
					while ((word = *str_row++) != nullptr && *word != '\0' && !(is_comment = mew::strcmp(word, ";"))) {
						Token tk;
						bool is_in_table = false;
						for (auto it = tokens.begin(); it != tokens.end(); ++it) {
							if (mew::strcmp(it->first, word)) {
								tk.type = it->second;
								if (use_beauty_error) tk.context = new Contexts::DefaultTokenContext(i);
								token_line.push(tk);
								is_in_table = true;
								break;
							}
						}
						if (!is_in_table) {
							tk.type = (TokenType)0;
							if (use_beauty_error) tk.context = new Contexts::DefaultTokenContext(i);
							tk.value = scopy(word);
							token_line.push(tk);
						}
					}
					if (token_line.size() == 0) continue;
					token_row.push(token_line.copy());
					if (is_comment) continue;
				}
				lines.clear();
			}	

			void _watch_token(Token& tk) {
				bool is_number;
				int i = str_to_int(tk.value, is_number);
				if (is_number) {
					tk.type = TokenType::Number,
					tk.value = (char*)i;
					return;
				}
				if (*tk.value == '"' && getLastChar(tk.value) == '"') {
					tk.type = TokenType::String;
					tk.context = new Contexts::String(strlen(tk.value)-2);
					mew::utils::fas(tk.value, (const char*)str_parse(tk.value+1, strlen(tk.value)-2));
					return;
				}
				if (watcher && watcher(tk)) return;
				tk.type = TokenType::Text;
			}

			void _decrypt_undefined() {
				for (int x = 0; x < token_row.size(); ++x) {
					auto& line = token_row[x];
					for (int y = 0; y < line.size(); ++y) {
						auto& e = line[y];
						if (e.type == (TokenType)0) {
							_watch_token(e);
						}
						(void)e;
					}
				}
			}
		};

		Lexer* tokenize(const char* input_content, const char* input_file = "local", bool use_beauty_error = false) {
			Lexer* lexer = new Lexer();
			lexer->input_file = input_file;
			lexer->lines = *utils::splitLines(input_content);
			lexer->use_beauty_error = use_beauty_error;
			lexer->_fill_simple();
			lexer->_decrypt_undefined();
			return lexer;
		}
		Lexer* tokenize(const char* input_content, token_watcher watcher, const char* input_file = "local", bool use_beauty_error = false) {
			Lexer* lexer = new Lexer();
			lexer->input_file = input_file;
			lexer->lines = *utils::splitLines(input_content);
			lexer->use_beauty_error = use_beauty_error;
			lexer->watcher = watcher;
			lexer->_fill_simple();
			lexer->_decrypt_undefined();
			return lexer;
		}
	};
		
};
#pragma pack(pop)


#endif
