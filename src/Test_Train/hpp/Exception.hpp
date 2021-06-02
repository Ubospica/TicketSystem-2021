//
// Created by Ubospica on 2021/4/23.
//

#ifndef BOOKSTORE_2021_EXCEPTION_HPP
#define BOOKSTORE_2021_EXCEPTION_HPP

#include <exception>
#include <utility>
#include <string>

namespace Ticket {
    class ExceptionBase : std::exception {
    private:
        std::string whatStr;
    public:
        explicit ExceptionBase(const std::string &str = std::string()) {
            if (!str.empty()) {
                whatStr = "Info: " + str;
            }
        }
        const char *what() const noexcept override {
            return whatStr.c_str();
        }
    };

    class NoOptionError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
    class SyntaxError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
    class RunningError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
    class PermissionError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
	class RuntimeError : public ExceptionBase {
		using ExceptionBase::ExceptionBase;
	};
    class WrongOperation : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
	
	// container exception
	class index_out_of_bound : public ExceptionBase {
		/* __________________________ */
	};
	
	class runtime_error : public ExceptionBase {
		/* __________________________ */
	};
	
	class invalid_iterator : public ExceptionBase {
		/* __________________________ */
	};
	
	class container_is_empty : public ExceptionBase {
		/* __________________________ */
	};
	
}

#endif //BOOKSTORE_2021_EXCEPTION_HPP
