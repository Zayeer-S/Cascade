#ifndef CASCADE_UTILS_VALIDATION_HPP
#define CASCADE_UTILS_VALIDATION_HPP

#pragma once
#include <string>
#include <vector>

namespace cascade
{
    struct ValidationResult
    {
        bool isValid = true;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;

        void addError(std::string msg)
        {
            isValid = false;
            errors.push_back(std::move(msg));
        }

        void addWarning(std::string msg) {
            warnings.push_back(std::move(msg));            
        }
    };
}

#endif