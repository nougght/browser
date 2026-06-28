#include <core/Url.h>
#include <ada.h>

// Url::Url(std::string value_) : value(value_)
// {
// }

std::optional<Url> Url::parse(std::string url)
{
    auto adaUrl = ada::parse(url);
    if (!adaUrl)
    {
        std::printf("Url::parse failed: %s\n", url.c_str());
        return std::nullopt;
    }
    Url result;
    result._protocol = adaUrl->get_protocol();
    result._hostname = adaUrl->get_hostname();
    result._port = adaUrl->get_port();
    result._path = adaUrl->get_pathname();
    result._query = adaUrl->get_search();
    result._fragment = adaUrl->get_hash();
    result._value = adaUrl->get_href();
    return result;
}

Url Url::newTabUrl(std::string path)
{
    auto adaUrl = ada::parse(NewTabUrl);
    if (!adaUrl) {
        return Url();
    }
    ada::url_search_params params;
    params.append("page", path);
    adaUrl->set_search(params.to_string());
    auto href = adaUrl->get_href();
    auto optionalUrl = Url::parse(std::string(href.begin(), href.end()));
    if (optionalUrl.has_value()) {
        return optionalUrl.value();
    }
    return Url();
}

std::optional<Url> Url::getNewTabUrl() {
    if (auto type = getInternalPageType(); type.has_value()) {
        if (type.value() == InternalPageType::NewTab) {
            auto query = _query;
            auto i = query.find('=');
            if (i == std::string::npos) {
                return std::nullopt;
            }
            auto url = query.erase(0, i + 1);
            return Url::parse(ada::unicode::percent_decode(url, url.find('%')));
        }
        return std::nullopt;
    }
    return std::nullopt;   
}

bool Url::operator==(const Url &other) const
{
    return _value == other._value;
}

std::string getInternalPageTypeTitle(InternalPageType type) {
    switch (type) {
        case InternalPageType::NewTab:
            return "New Tab";
        case InternalPageType::History:
            return "History";
        case InternalPageType::Bookmarks:
            return "Bookmarks";
        case InternalPageType::Settings:
            return "Settings";
        default:
            return "";
    }
}

std::optional<Url> getInternalPageTypeUrl(InternalPageType type, Url newTabPageUrl) {

    switch (type) {
    case InternalPageType::NewTab:
        if (newTabPageUrl.isEmpty()) {
            return std::nullopt;
        }
        return Url::newTabUrl(newTabPageUrl.toStdString());
    case InternalPageType::History:
        return Url::historyUrl();
    case InternalPageType::Bookmarks:
        return Url::bookmarksUrl();
    case InternalPageType::Settings:
        return Url::settingsUrl();
    default:
        return std::nullopt;
    }
}

bool isLikeUrl(std::string query) {
    if (query.find(" ") != std::string::npos) {
        return false;
    }
    if (query.find(".") == std::string::npos) {
        return false;
    }
    
    return true;
}
