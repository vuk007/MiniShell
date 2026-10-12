#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <vector>

class CLIException : public std::exception {
public:
    explicit CLIException(std::string message) : message_(std::move(message)) {}
    const char* what() const noexcept override { return message_.c_str(); }
protected:
    std::string message_;
};

class LexicalError : public CLIException {
public:
    LexicalError(const std::string& commandLine, const std::vector<int>& errorPositions)
        : CLIException(buildMessage(commandLine, errorPositions)),
          commandLine_(commandLine), errorPositions_(errorPositions) {}
    LexicalError(const std::string& commandLine, int singlePosition)
        : LexicalError(commandLine, singlePosition >= 0 ? std::vector<int>{singlePosition} : std::vector<int>()) {}
    const std::string& getCommandLine() const { return commandLine_; }
    const std::vector<int>& getErrorPositions() const { return errorPositions_; }
private:
    std::string commandLine_;
    std::vector<int> errorPositions_;
    static std::string buildMessage(const std::string& commandLine, const std::vector<int>& errorPositions) {
        std::string msg = "Error - neprepoznatljivi karakteri:\n" + commandLine + "\n";
        std::string caretLine(commandLine.size(), ' ');
        for (int pos : errorPositions) {
            if (pos >= 0 && pos < (int)commandLine.size())
                caretLine[pos] = '^';
        }
        return msg + caretLine;
    }
};

class UnknownCommandError : public CLIException {
public:
    explicit UnknownCommandError(const std::string& commandName)
        : CLIException("Nepoznata komanda: " + commandName), commandName_(commandName) {}
    const std::string& getCommandName() const { return commandName_; }
private:
    std::string commandName_;
};

class NoCommandError : public CLIException {
public:
    NoCommandError() : CLIException("(no command)") {}
};

class SyntaxError : public CLIException {
public:
    explicit SyntaxError(const std::string& message) : CLIException(message) {}
};

class EmptyPipelineCommandError : public CLIException {
public:
    EmptyPipelineCommandError() : CLIException("Error: prazan pipeline") {}
};

class PipelineStreamConflictError : public CLIException {
public:
    PipelineStreamConflictError() : CLIException("Error: komanda ima dodatni input_stream argument") {}
};

class CommandSemanticError : public CLIException {
public:
    explicit CommandSemanticError(const std::string& message) : CLIException(message) {}
};

class TouchMissingFilenameError : public CLIException {
public:
    TouchMissingFilenameError() : CLIException("Error: touchfilename argument fali") {}
};

class TruncateMissingFilenameError : public CLIException {
public:
    TruncateMissingFilenameError() : CLIException("Error: truncate filename argument fali") {}
};

class RmMissingFilenameError : public CLIException {
public:
    RmMissingFilenameError() : CLIException("Error: rm filename argument fali") {}
};

class BatchMissingFilenameError : public CLIException {
public:
    BatchMissingFilenameError() : CLIException("Error: batch filename argument fali") {}
};

class WcMissingOptionError : public CLIException {
public:
    WcMissingOptionError() : CLIException("Error: wc opcija fali [-w ili -c]") {}
};

class WcNoInputError : public CLIException {
public:
    WcNoInputError() : CLIException("Error: wc neuspesno citanje fajla") {}
};

class TrMissingWhatError : public CLIException {
public:
    TrMissingWhatError() : CLIException("Error: tr potreban –what argument[ –\"string\"]") {}
};

class HeadInvalidOptionError : public CLIException {
public:
    HeadInvalidOptionError() : CLIException("Error: head -n argument fali ") {}
};

class HeadNoInputError : public CLIException {
public:
    HeadNoInputError() : CLIException("Error: head fali input argument") {}
};

class FileExistsError : public CLIException {
public:
    explicit FileExistsError(const std::string& filename)
        : CLIException("Error: file '" + filename + "' already exists."), filename_(filename) {}
    const std::string& getFilename() const { return filename_; }
private:
    std::string filename_;
};

class FileNotFoundError : public CLIException {
public:
    explicit FileNotFoundError(const std::string& filename)
        : CLIException("Error: file '" + filename + "' not found."), filename_(filename) {}
    const std::string& getFilename() const { return filename_; }
private:
    std::string filename_;
};

class FileOpenError : public CLIException {
public:
    explicit FileOpenError(const std::string& filename)
        : CLIException("Error: could not open file '" + filename + "'."), filename_(filename) {}
    const std::string& getFilename() const { return filename_; }
private:
    std::string filename_;
};

class FileCreateError : public CLIException {
public:
    explicit FileCreateError(const std::string& filename)
        : CLIException("Error: could not create file '" + filename + "'."), filename_(filename) {}
    const std::string& getFilename() const { return filename_; }
private:
    std::string filename_;
};

class FileRemoveError : public CLIException {
public:
    explicit FileRemoveError(const std::string& filename)
        : CLIException("Error: could not remove file '" + filename + "'."), filename_(filename) {}
    const std::string& getFilename() const { return filename_; }
private:
    std::string filename_;
};

class FileTruncateError : public CLIException {
public:
    explicit FileTruncateError(const std::string& filename)
        : CLIException("Error: could not truncate file '" + filename + "'."), filename_(filename) {}
    const std::string& getFilename() const { return filename_; }
private:
    std::string filename_;
};

#endif
