#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    explicit Logger(std::string filename);

    void logMessage(const std::string &message, const std::string &importance) const;

private:
    std::string logFilename;
};

#endif // LOGGER_H