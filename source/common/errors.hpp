#pragma once
#include <iostream>

enum class ErrorCode {
    //for class vector
    IndexOutOfRange = 101,
    EmptyContainer = 102,
    NegativeSize = 103,
    SelfAssignment = 104,
    CapacityOverflow = 105,
    ValueNotFound = 106,

    //for files
    FileNotFound = 201,
    FileOpenFailed = 202,
    FileWriteFailed = 203,
    ParseError = 204,
    EmptyFile = 205,
    CorruptedData = 206,

    //for errors with data
    InvalidArgument = 301,
    InvalidTask = 302,
    TaskNotFound = 303,
    InvalidId = 304,
    InvalidDuration = 305,
    InvalidVolume = 306,
    InvalidRange = 307,
    InvalidState = 308,
    InvalidDate = 309,
    InvalidPeriod = 310,
    UnknownSetting = 311,

    //for audio
    AudioFileError = 401,
    AudioDeviceError = 402,
    UnsupportedFormat = 403,
    EmptyPlaylist = 404,
    TrackNotFound = 405,

    //graphics
    AssetNotFound = 501,
    FontLoadFailed = 502,
    TextureLoadFailed = 503,
    WindowCreateFailed = 504,
};

class errors {
protected:
    std::string message;
    ErrorCode code;
public:
    errors(ErrorCode c, const std::string& text) { message = text; code = c; }
    virtual ~errors() {}
    std::string msg() { return message; }
    ErrorCode getCode() const { return code; }
};

class ContainerError : public errors {
public:
    ContainerError(ErrorCode c, const std::string& text) : errors(c, "Container: " + text) {}
};

class FileError : public errors {
public:
    FileError(ErrorCode c, const std::string& text) : errors(c, "File: " + text) {}
};

class DomainError : public errors {
public:
    DomainError(ErrorCode c, const std::string& text) : errors(c, "Logic: " + text) {}
};

class AudioError : public errors {
public:
    AudioError(ErrorCode c, const std::string& text) : errors(c, "Audio: " + text) {}
};

class AssetError : public errors {
public:
    AssetError(ErrorCode c, const std::string& text) : errors(c, "Asset: " + text) {}
};

// exact errors
class FileNotFound : public FileError {
public:
    FileNotFound() : FileError(ErrorCode::FileNotFound, "file not found") {}
};

class FileOpenFailed : public FileError {
public:
    FileOpenFailed() : FileError(ErrorCode::FileOpenFailed, "failed to open file") {}
};

class FileWriteFailed : public FileError {
public:
    FileWriteFailed() : FileError(ErrorCode::FileWriteFailed, "failed to write file") {}
};

class ParseError : public FileError {
public:
    ParseError() : FileError(ErrorCode::ParseError, "string parsing error") {}
};

class EmptyFile : public FileError {
public:
    EmptyFile() : FileError(ErrorCode::EmptyFile, "file is empty") {}
};

class CorruptedData : public FileError {
public:
    CorruptedData() : FileError(ErrorCode::CorruptedData, "data is corrupted") {}
};

class InvalidArgument : public DomainError {
public:
    InvalidArgument() : DomainError(ErrorCode::InvalidArgument, "invalid argument") {}
};

class InvalidTask : public DomainError {
public:
    InvalidTask() : DomainError(ErrorCode::InvalidTask, "invalid task") {}
};

class TaskNotFound : public DomainError {
public:
    TaskNotFound() : DomainError(ErrorCode::TaskNotFound, "task not found") {}
};

class InvalidId : public DomainError {
public:
    InvalidId() : DomainError(ErrorCode::InvalidId, "invalid id") {}
};

class InvalidDuration : public DomainError {
public:
    InvalidDuration() : DomainError(ErrorCode::InvalidDuration, "invalid duration") {}
};

class InvalidVolume : public DomainError {
public:
    InvalidVolume() : DomainError(ErrorCode::InvalidVolume, "volume out of range") {}
};

class InvalidRange : public DomainError {
public:
    InvalidRange() : DomainError(ErrorCode::InvalidRange, "invalid range") {}
};

class InvalidState : public DomainError {
public:
    InvalidState() : DomainError(ErrorCode::InvalidState, "operation impossible in current state") {}
};

class InvalidDate : public DomainError {
public:
    InvalidDate() : DomainError(ErrorCode::InvalidDate, "invalid date") {}
};

class InvalidPeriod : public DomainError {
public:
    InvalidPeriod() : DomainError(ErrorCode::InvalidPeriod, "unsupported period") {}
};

class UnknownSetting : public DomainError {
public:
    UnknownSetting() : DomainError(ErrorCode::UnknownSetting, "unknown setting key") {}
};

class AudioFileError : public AudioError {
public:
    AudioFileError() : AudioError(ErrorCode::AudioFileError, "failed to load audio file") {}
};

class AudioDeviceError : public AudioError {
public:
    AudioDeviceError() : AudioError(ErrorCode::AudioDeviceError, "audio device unavailable") {}
};

class UnsupportedFormat : public AudioError {
public:
    UnsupportedFormat() : AudioError(ErrorCode::UnsupportedFormat, "format not supported") {}
};

class EmptyPlaylist : public AudioError {
public:
    EmptyPlaylist() : AudioError(ErrorCode::EmptyPlaylist, "playlist is empty") {}
};

class TrackNotFound : public AudioError {
public:
    TrackNotFound() : AudioError(ErrorCode::TrackNotFound, "track not found") {}
};

class AssetNotFound : public AssetError {
public:
    AssetNotFound() : AssetError(ErrorCode::AssetNotFound, "asset not found") {}
};

class FontLoadFailed : public AssetError {
public:
    FontLoadFailed() : AssetError(ErrorCode::FontLoadFailed, "failed to load font") {}
};

class TextureLoadFailed : public AssetError {
public:
    TextureLoadFailed() : AssetError(ErrorCode::TextureLoadFailed, "failed to load image") {}
};

class WindowCreateFailed : public AssetError {
public:
    WindowCreateFailed() : AssetError(ErrorCode::WindowCreateFailed, "failed to create window") {}
};

// for array
class IndexOutOfRange : public ContainerError {
public:
    IndexOutOfRange() : ContainerError(ErrorCode::IndexOutOfRange, "index out of range") {}
};

class EmptyContainer : public ContainerError {
public:
    EmptyContainer() : ContainerError(ErrorCode::EmptyContainer, "container is empty") {}
};

class NegativeSize : public ContainerError {
public:
    NegativeSize() : ContainerError(ErrorCode::NegativeSize, "negative size") {}
};

class SelfAssignment : public ContainerError {
public:
    SelfAssignment() : ContainerError(ErrorCode::SelfAssignment, "self assignment") {}
};

class CapacityOverflow : public ContainerError {
public:
    CapacityOverflow() : ContainerError(ErrorCode::CapacityOverflow, "capacity overflow") {}
};

class ValueNotFound : public ContainerError {
public:
    ValueNotFound() : ContainerError(ErrorCode::ValueNotFound, "value not found") {}
};
