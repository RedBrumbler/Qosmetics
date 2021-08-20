#pragma once

#include <string>
#include <functional>

namespace WebUtils
{
    /// @brief gets the contents of url and passes them into the finished callback along with the http response code, calls progressUpdate with progressupdates if given
    /// @param url the url to query
    /// @param finished the callback for when we're done downloading
    /// @param progressUpdate callback to give a progressupdate bar or something
    void GetAsync(std::string url, std::function<void(long, std::string)> finished, std::function<void(float)> progressUpdate = nullptr);

    /// @brief gets the contents of url and passes them into the finished callback along with the http response code, calls progressUpdate with progressupdates if given
    /// @param url the url to query
    /// @param timeout the timeout for the query
    /// @param finished the callback for when we're done downloading
    /// @param progressUpdate callback to give a progressupdate bar or something
    void GetAsync(std::string url, long timeout, std::function<void(long, std::string)> finished, std::function<void(float)> progressUpdate = nullptr);
}