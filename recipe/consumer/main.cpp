#include <azure/core/url.hpp>
#include <iostream>
int main() {
    Azure::Core::Url url("https://example.com:8443/container/blob?key=value");
    if (url.GetScheme() != "https" || url.GetHost() != "example.com" ||
        url.GetPort() != 8443 || url.GetPath() != "container/blob" ||
        url.GetQueryParameters().at("key") != "value") return 1;
    url.AppendPath("child");
    Azure::Core::Url copy(url.GetAbsoluteUrl());
    if (copy.GetPath() != "container/blob/child" || copy.GetPort() != 8443)
        return 2;
    std::cout << "Installed Azure Core URL parsing and reconstruction passed\n";
}
