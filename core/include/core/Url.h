#ifndef URL_H
#define URL_H

#include <fstream>
#include <ostream>
#include <string>
#include <optional>


// base urls

const std::string GoogleBaseUrl = "https://www.google.com";
const std::string YandexBaseUrl = "https://yandex.com";
const std::string DuckDuckGoBaseUrl = "https://duckduckgo.com";

// search paths

const std::string GoogleSearchPath = "/search";
const std::string YandexSearchPath = "/search";
const std::string DuckDuckGoSearchPath = "";

// search query params

const std::string GoogleSearchQueryParam = "q";
const std::string YandexSearchQueryParam = "text";
const std::string DuckDuckGoSearchQueryParam = "q";


// internal page urls

// new tab page url
const std::string NewTabUrl = "br://newtab";
// history page url
const std::string HistoryUrl = "br://history";
// bookmarks page url
const std::string BookmarksUrl = "br://bookmarks";
// settings page url
const std::string SettingsUrl = "br://settings";

// internal page types
enum class InternalPageType {
    NewTab,
    History,
    Bookmarks,
    Settings,
};

bool isLikeUrl(std::string query);

class Url
{
private:
    std::string _value;

    std::string _protocol;
    std::string _hostname;
    std::string _port;
    std::string _path;
    std::string _query;
    std::string _fragment;
    
public:
    Url() : _value(""), _protocol(""), _hostname(""), _port(""), _path(""), _query(""), _fragment("") {}
    Url(const Url& other) : _value(other._value), _protocol(other._protocol), _hostname(other._hostname), _port(other._port), _path(other._path), _query(other._query), _fragment(other._fragment) {}
    static std::optional<Url> parse(std::string value);

    static Url newTabUrl(std::string path = "");
    static Url historyUrl()
    {
        return Url::parse(HistoryUrl).value();
    }
    static Url bookmarksUrl()
    {
        return Url::parse(BookmarksUrl).value();
    }
    static Url settingsUrl()
    {
        return Url::parse(SettingsUrl).value();
    }

    bool operator==(const Url& other) const;
    
    std::string toStdString() const
    {
        return _value;
    }

    bool isEmpty() const
    {
        return _value.empty();
    }

    bool isInternal() const
    {
        return _protocol == "br:";
    }

    std::optional<InternalPageType> getInternalPageType() const 
    {
        if (!isInternal())
            return std::nullopt;
        
        if (_value.find(NewTabUrl) == 0) 
            return InternalPageType::NewTab;
        if (_value == HistoryUrl)
            return InternalPageType::History;
        if (_value == BookmarksUrl)
            return InternalPageType::Bookmarks;
        if (_value == SettingsUrl)
            return InternalPageType::Settings;
        return std::nullopt;
    }

    std::optional<Url> getNewTabUrl();

};

inline std::ostream& operator<<(std::ostream &out, Url url) {
    out << url.toStdString();
}

std::string getInternalPageTypeTitle(InternalPageType type);
std::optional<Url> getInternalPageTypeUrl(InternalPageType type, Url newTabPageUrl = Url());
#endif
