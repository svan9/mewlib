#ifndef MEW_ISOLATE_HPP
#define MEW_ISOLATE_HPP

#include "mewlib.h"
#include "mewtypes.h"
#include "mewmap.hpp"
#include "mewstack.hpp"
#include <stdio.h>
#include <stdbool.h>
#include "mewpack"
#include <variant>

namespace mew {

	struct IOFlags {
		bytepartf(Read)
		bytepartf(Write)
		bytepartf(Binary)
		bytepartf(Append)
		bytepartf(Create)
		bytepartf(IsIsolate)

		constexpr IOFlags() : Read(false), Write(false), Binary(false), Create(false), IsIsolate(false), Append(false) {}
		constexpr IOFlags(bool read, bool write, bool binary, bool append, bool create, bool is_isolate = false)
			: Read(read), Write(write), Binary(binary), Create(create), IsIsolate(is_isolate), Append(append) {}

		IOFlags(const char* flags) {
			size_t len = strlen(flags);
			if (len >= 5) {
				Read = (flags[0] == 'r') || (flags[1] == 'r') || (flags[2] == 'r') || (flags[3] == 'r') 
					|| (flags[4] == 'r'); 
				Write = (flags[0] == 'w') || (flags[1] == 'w') || (flags[2] == 'w') || (flags[3] == 'w') 
					|| (flags[4] == 'w');
				Binary = (flags[0] == 'b') || (flags[1] == 'b') || (flags[2] == 'b') || (flags[3] == 'b') 
					|| (flags[4] == 'b');
				Create = (flags[0] == '+') || (flags[1] == '+') || (flags[2] == '+') || (flags[3] == '+') 
					|| (flags[4] == '+');
				IsIsolate = (flags[0] == 'i') || (flags[1] == 'i') || (flags[2] == 'i') || (flags[3] == 'i') 
					|| (flags[4] == 'i');
				Append = (flags[0] == 'a') || (flags[1] == 'a') || (flags[2] == 'a') || (flags[3] == 'a') 
					|| (flags[4] == 'a');
				} else if (len >= 4) {
					Read = (flags[0] == 'r') || (flags[1] == 'r') || (flags[2] == 'r') || (flags[3] == 'r'); 
					Write = (flags[0] == 'w') || (flags[1] == 'w') || (flags[2] == 'w') || (flags[3] == 'w');
					Binary = (flags[0] == 'b') || (flags[1] == 'b') || (flags[2] == 'b') || (flags[3] == 'b');
					Create = (flags[0] == '+') || (flags[1] == '+') || (flags[2] == '+') || (flags[3] == '+');
					IsIsolate = (flags[0] == 'i') || (flags[1] == 'i') || (flags[2] == 'i') || (flags[3] == 'i');
					Append = (flags[0] == 'a') || (flags[1] == 'a') || (flags[2] == 'a') || (flags[3] == 'a');
				} else if (len >= 3) {
					Read = (flags[0] == 'r') || (flags[1] == 'r') || (flags[2] == 'r'); 
					Write = (flags[0] == 'w') || (flags[1] == 'w') || (flags[2] == 'w');
					Binary = (flags[0] == 'b') || (flags[1] == 'b') || (flags[2] == 'b');
					Create = (flags[0] == '+') || (flags[1] == '+') || (flags[2] == '+');
					IsIsolate = (flags[0] == 'i') || (flags[1] == 'i') || (flags[2] == 'i');
					Append = (flags[0] == 'a') || (flags[1] == 'a') || (flags[2] == 'a');
				} else if (len >= 2) {
					Read = (flags[0] == 'r') || (flags[1] == 'r'); 
					Write = (flags[0] == 'w') || (flags[1] == 'w');
					Binary = (flags[0] == 'b') || (flags[1] == 'b');
					Create = (flags[0] == '+') || (flags[1] == '+');
					IsIsolate = (flags[0] == 'i') || (flags[1] == 'i');
					Append = (flags[0] == 'a') || (flags[1] == 'a');
				} else {
				MewUserAssert(false, "Invalid IOFlags string");
			}
		}

		const char* Raw() {
			static char mode[8];
			int i = 0;
			if (Read && Write) {
				mode[i++] = 'w';
				mode[i++] = '+';
			} else if (Read) {
				mode[i++] = 'r';
			} else if (Write) {
				mode[i++] = 'w';
			}
			if (Append) {
				mode[0] = 'a';
			}
			if (Binary) {
				mode[i++] = 'b';
			}
			mode[i] = '\0';
			return mode;
			
		}

	};

	struct IsolateFile {
		mew::stack<u8> data;
		IOFlags flags;
	};
	
	using RawFile = std::variant
		<std::monostate, IsolateFile*, FILE*>;
	
	class Isolate {	
	private:
		typedef mew::hashmap<IsolateFile> isolate_disk_t;
		bool m_is_isolate = false;
		isolate_disk_t m_space;
	public:
		Isolate() {}
		Isolate(bool is_isolate): m_is_isolate(is_isolate) {}

		bool toggle() {
			m_is_isolate = !m_is_isolate;
			return m_is_isolate;
		}

		bool off() {
			m_is_isolate = false;
			return m_is_isolate;
		}

		bool on() {
			m_is_isolate = true;
			return m_is_isolate;
		}

		bool IsExist(const char* path) {
			if (m_is_isolate) {
				return m_space.contains(path);
			} else {
				FILE *fp = fopen(path, "r");
				bool is_exist = false;
				if (fp != NULL) {
					is_exist = true;
					fclose(fp);
				}
				return is_exist;
			}
		}

#ifdef _WIN32
		bool IsExist(const wchar_t* path) {
			if (m_is_isolate) {
				return m_space.contains(path);
			} else {
				FILE *fp = _wfopen(path, L"r");
				bool is_exist = false;
				if (fp != NULL) {
					is_exist = true;
					fclose(fp);
				}
				return is_exist;
			}
		}
#endif

		bool IsFile(const char* path) {
			if (m_is_isolate) {
				return m_space.contains(path);
			} else {
				FILE* fp = fopen(path, "rb");
				if (fp == nullptr) return false;
				bool is_file = (fseek(fp, 0, SEEK_SET) == 0);
				fclose(fp);
				return is_file;
			}
		}
		
		bool IsFile(const wchar_t* path) {
			if (m_is_isolate) {
				return m_space.contains(path);
			} else {
				FILE* fp = _wfopen(path, L"rb");
				if (fp == nullptr) return false;
				bool is_file = (fseek(fp, 0, SEEK_SET) == 0);
				fclose(fp);
				return is_file;
			}
		}

		void WriteToFile(const char* path, const char* content) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				file.data.resize(strlen(content));
				memcpy(file.data.begin(), content, strlen(content));
				return;
			}
			FILE* fp = fopen(path, "wb");
			if (fp == nullptr) {
				return;
			}
			fwrite(content, 1, strlen(content), fp);
			fclose(fp);
		}

		void WriteToFile(const char* path, const char* content, size_t& cursor) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				if (cursor >= file.data.size()) {
					file.data.resize(cursor + strlen(content));
				}
				memcpy(file.data.begin() + cursor, content, strlen(content));
				cursor += strlen(content);
				return;
			}
			FILE* fp = fopen(path, "r+b");
			if (fp == nullptr) {
				return;
			}
			fseek(fp, cursor, SEEK_SET);
			fwrite(content, 1, strlen(content), fp);
			fclose(fp);
			cursor += strlen(content);
		}

		void WriteToFile(const wchar_t* path, const char* content, size_t& cursor) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				if (cursor >= file.data.size()) {
					file.data.resize(cursor + strlen(content));
				}
				memcpy(file.data.begin() + cursor, content, strlen(content));
				cursor += strlen(content);
				return;
			}
			FILE* fp = _wfopen(path, L"r+b");
			if (fp == nullptr) {
				return;
			}
			fseek(fp, cursor, SEEK_SET);
			fwrite(content, 1, strlen(content), fp);
			fclose(fp);
			cursor += strlen(content);
		}

		size_t GetFileSize(const char* path) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				return file.data.size();
			}
			FILE* fp = fopen(path, "rb");
			if (fp == nullptr) {
				return 0;
			}
			fseek(fp, 0, SEEK_END);
			size_t size = ftell(fp);
			fclose(fp);
			return size;
		}

		size_t GetFileSize(const wchar_t* path) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				return file.data.size();
			}
			FILE* fp = _wfopen(path, L"rb");
			if (fp == nullptr) {
				return 0;
			}
			fseek(fp, 0, SEEK_END);
			size_t size = ftell(fp);
			fclose(fp);
			return size;
		}

		const char* ReadFromFile(const char* path, size_t* fsize = nullptr) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				if (fsize) { *fsize = file.data.size();}
				return (const char*)file.data.begin();
			}
			FILE* fp = fopen(path, "rb");
			if (fp == nullptr) {
				return nullptr;
			}
			fseek(fp, 0, SEEK_END);
			long size = ftell(fp);
			if (fsize) { *fsize = size; }
			rewind(fp);
			char* buffer = new char[size + 1];
			size_t read_size = fread(buffer, 1, size, fp);
			buffer[read_size] = '\0';
			fclose(fp);
			return buffer;
		}
		const char* ReadFromFile(const wchar_t* path, size_t* fsize = nullptr) {
			MewUserAssert(IsFile(path), "is not a path");
			if (m_is_isolate) {
				auto& file = m_space.at(path);
				if (fsize) { *fsize = file.data.size();}
				return (const char*)file.data.begin();
			}
			FILE* fp = _wfopen(path, L"rb");
			if (fp == nullptr) {
				return nullptr;
			}
			fseek(fp, 0, SEEK_END);
			long size = ftell(fp);
			if (fsize) { *fsize = size; }
			rewind(fp);
			char* buffer = new char[size + 1];
			size_t read_size = fread(buffer, 1, size, fp);
			buffer[read_size] = '\0';
			fclose(fp);
			return buffer;
		}
		
		void CreateFileIfNotExist(const char* path) {
			if (IsExist(path)) return;
			if (m_is_isolate) {
				IsolateFile file;
				m_space.insert(path, file);
			} else {
				FILE* fp = fopen(path, "wb");
				if (fp != nullptr) {
					fclose(fp);
				}
			}
		}

		void CreateFileIfNotExist(const wchar_t* path) {
			if (IsExist(path)) return;
			if (m_is_isolate) {
				IsolateFile file;
				m_space.insert(path, file);
			} else {
				FILE* fp = _wfopen(path, L"wb");
				if (fp != nullptr) {
					fclose(fp);
				}
			}
		}

		RawFile Open(const char* path, const char* flags) {
			IOFlags _ioflags(flags);
			if (m_is_isolate) {
				if (IsExist(path)) {
					return RawFile{&m_space.at(path)};
				}
				if (_ioflags.Create) {
					IsolateFile file;
					return RawFile{&m_space.insert(path, file)};
				}
				return RawFile{};
			}
			FILE* fp = fopen(path, flags);
			if (fp != nullptr) {
				fclose(fp);
			}
			return RawFile{fp};
		}

		RawFile Open(const wchar_t* path, const char* flags) {
			IOFlags _ioflags(flags);
			if (m_is_isolate) {
				if (IsExist(path)) {
					auto file = &m_space.at(path);
					file->flags = _ioflags;
					return RawFile{file};
				}
				if (_ioflags.Create) {
					IsolateFile file;
					file.flags = _ioflags;
					return RawFile{&m_space.insert(path, file)};
				}
				return RawFile{};
			}
			FILE* fp = _wfopen(path, char_to_wchar(flags));
			if (fp != nullptr) {
				fclose(fp);
			}
			return RawFile{fp};
		}

		void Close(RawFile rf) {
			std::visit([](auto&& arg) { 
				using T = std::decay_t<decltype(arg)>;
				// if constexpr (std::is_same_v<T, IsolateFile*>) -- NO REASON TO CLOSE ISOLATE FILE
				if constexpr (std::is_same_v<T, FILE*>) {
					if (arg != nullptr) {
						fclose(arg);
					}
				}
			 }, rf);
		}

		// Read data from RawFile into buffer, up to size bytes. Returns number of bytes read.
		size_t Read(RawFile rf, char* buffer, size_t size, size_t offset = 0) {
			return std::visit([&](auto&& arg) -> size_t {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, IsolateFile*>) {
					if (!arg) return 0;
					if (!arg->flags.Read) { return 0; }
					size_t available = arg->data.size();
					if (offset >= available) return 0;
					size_t to_read = std::min(size, available - offset);
					memcpy(buffer, arg->data.begin() + offset, to_read);
					return to_read;
				} else if constexpr (std::is_same_v<T, FILE*>) {
					if (!arg) return 0;
					if (fseek(arg, offset, SEEK_SET) != 0) return 0;
					return fread(buffer, 1, size, arg);
				}
				return 0;
			}, rf);
		}

		// Write data from buffer into RawFile, up to size bytes. Returns number of bytes written.
		size_t Write(RawFile rf, const char* buffer, size_t size, size_t offset = 0) {
			return std::visit([&](auto&& arg) -> size_t {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, IsolateFile*>) {
					if (!arg) return 0;
					if (!arg->flags.Write) { return 0; }
					if (offset + size > arg->data.size()) {
						arg->data.resize(offset + size);
					}
					memcpy(arg->data.begin() + offset, buffer, size);
					return size;
				} else if constexpr (std::is_same_v<T, FILE*>) {
					if (!arg) return 0;
					if (fseek(arg, offset, SEEK_SET) != 0) return 0;
					return fwrite(buffer, 1, size, arg);
				}
				return 0;
			}, rf);
		}

		size_t GetFileSize(RawFile rf) {
			return std::visit([](auto&& arg) -> size_t {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, IsolateFile*>) {
					if (!arg) return 0;
					return arg->data.size();
				} else if constexpr (std::is_same_v<T, FILE*>) {
					if (!arg) return 0;
					long current_pos = ftell(arg);
					fseek(arg, 0, SEEK_END);
					long size = ftell(arg);
					fseek(arg, current_pos, SEEK_SET);
					return size;
				}
				return 0;
			}, rf);
		}

		
	};
};

#include "mewpop"

#endif