#include "HTTPRequest.hpp"
#include <iostream>
#include <thread>
#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
class eauth
{
protected:
    const std::string ApplicationKey = "";
    const std::string AccountKey = "";

    bool rep(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }
    HWND winHWND = nullptr;
    std::string hPass(std::string altashfir)
    {
        http::Request request{ "http://eauth.000webhostapp.com/api/" };
        const std::string body = "0=x&O=" + altashfir;
        const auto response = request.send("POST", body, {
            {"Content-Type", "application/x-www-form-urlencoded"}
            });
        return std::string{ response.body.begin(), response.body.end() };
    }
    std::string decJSON(std::string altashfir, std::string given)
    {
        http::Request request{ "http://eauth.000webhostapp.com/api/" };
        const std::string body = "0=c&O=" + altashfir + "&Y=" + given;
        const auto response = request.send("POST", body, {
            {"Content-Type", "application/x-www-form-urlencoded"}
            });
        return std::string{ response.body.begin(), response.body.end() };
    }
    std::string wahid(std::string altashfir)
    {
        http::Request request{ "http://eauth.000webhostapp.com/api/" };
        const std::string body = "0=e&O=" + altashfir;
        const auto response = request.send("POST", body, {
            {"Content-Type", "application/x-www-form-urlencoded"}
            });
        return std::string{ response.body.begin(), response.body.end() };
    }
    std::string sifr(std::string altashfir)
    {
        http::Request request{ "http://eauth.000webhostapp.com/api/" };
        const std::string body = "0=d&O=" + altashfir;
        const auto response = request.send("POST", body, {
            {"Content-Type", "application/x-www-form-urlencoded"}
            });
        return std::string{ response.body.begin(), response.body.end() };
    }
private:
    inline static std::string PAUSED;
    inline static bool initB;
public:
    inline static std::string LoggedMessage;
    inline static std::string RegisteredMessage;
    inline static std::string ApplicationName;
    inline static std::string NAME;
    inline static std::string RANK;
    inline static std::string CREATEDATE;
    inline static std::string EXPIREDATE;
    HW_PROFILE_INFO hwProfileInfo;
    std::string HWID() {
        std::string guid_f;
        GetCurrentHwProfile(&hwProfileInfo);
        guid_f = hwProfileInfo.szHwProfileGuid;
        rep(guid_f, "{", "");
        rep(guid_f, "}", "");
        rep(guid_f, " ", "");
        return guid_f;
    }
	void init()
	{
        try
        {
            if (!initB)
            {
                http::Request request{ "http://eauth.000webhostapp.com/api/" };
                const std::string body = "s0rt=" + wahid("init") + "&111110=" + wahid(ApplicationKey) + " &001011=" + wahid(AccountKey) + "&011001=" + wahid(HWID());
                const auto response = request.send("POST", body, {
                    {"Content-Type", "application/x-www-form-urlencoded"}
                    });
                if (sifr(std::string{ response.body.begin(), response.body.end() }) == "incorrect_application_details")
                {
                    MessageBox(winHWND, "Incorrect application details!", "", MB_OK | MB_ICONINFORMATION);
                    exit(1);
                }
                else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "banned_user")
                {
                    exit(0);
                }
                std::string APP_INFO = sifr(std::string{ response.body.begin(), response.body.end() });
                rep(APP_INFO, "\\", "");
                if (decJSON(APP_INFO, "STATUS") == "0")
                {
                    exit(1);
                }
                else
                {
                    ApplicationName = decJSON(APP_INFO, "APPNAME");
                    LoggedMessage = decJSON(APP_INFO, "LOGGED");
                    RegisteredMessage = decJSON(APP_INFO, "REGISTERED");
                    PAUSED = decJSON(APP_INFO, "PAUSED");
                    initB = true;
                }
            }
        }
        catch (const std::exception& e)
        {
            exit(0);
        }
	}
    bool signin(std::string username, std::string password)
    {
        try
        {
            http::Request request{ "http://eauth.000webhostapp.com/api/" };
            const std::string body = "s0rt="+wahid("l0gin")+"&username="+wahid(username)+ " &passw0rd=" + wahid(hPass(password)) + "&hwid=" + wahid(HWID()) + "&appkey=" + wahid(ApplicationKey) + "&acckey=" + wahid(AccountKey);
            const auto response = request.send("POST", body, {
                {"Content-Type", "application/x-www-form-urlencoded"}
                });
            if (sifr(std::string{response.body.begin(), response.body.end()}) == "incorrect_login_details")
            {
                MessageBox(winHWND, "Incorrect login details!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "incorrect_user_details")
            {
                MessageBox(winHWND, "Incorrect login details!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "hwid_does_not_match")
            {
                MessageBox(winHWND, "Hwid doesn't match!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "subscription_has_expired")
            {
                MessageBox(winHWND, "Your subscription has expired!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else
            {
                std::string ACC_INFO = sifr(std::string{ response.body.begin(), response.body.end() });
                rep(ACC_INFO, "\\", "");
                NAME = decJSON(ACC_INFO, "NAME");
                RANK = decJSON(ACC_INFO, "RANKUSER");
                CREATEDATE = decJSON(ACC_INFO, "CREATEDATE");
                EXPIREDATE = decJSON(ACC_INFO, "EXPIREDATE");
                return true;
            }
        }
        catch (const std::exception& e)
        {
            exit(0);
        }
    }
    bool signup(std::string username, std::string password, std::string invite)
    {
        try
        {
            http::Request request{ "http://eauth.000webhostapp.com/api/" };
            const std::string body = "s0rt=" + wahid("register") + "&username=" + wahid(username) + " &passw0rd=" + wahid(hPass(password)) + "&invite=" + wahid(invite) + "&hwid=" + wahid(HWID()) + "&appkey=" + wahid(ApplicationKey) + "&acckey=" + wahid(AccountKey);
            const auto response = request.send("POST", body, {
                {"Content-Type", "application/x-www-form-urlencoded"}
                });
            if (sifr(std::string{ response.body.begin(), response.body.end() }) == "name_already_used")
            {
                MessageBox(winHWND, "Name already used!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "incorrect_register_details")
            {
                MessageBox(winHWND, "Incorrect register details!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "key_not_found")
            {
                MessageBox(winHWND, "Key not found!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "maximum_users")
            {
                MessageBox(winHWND, "The Application reached maximum users, it's time for an upgrade!", "", MB_OK | MB_ICONINFORMATION);
                return false;
            }
            else
            {
                return true;
            }
        }
        catch (const std::exception& e)
        {
            exit(0);
        }
    }
    std::string grabVariable(std::string varid)
    {
        try
        {
            http::Request request{ "http://eauth.000webhostapp.com/api/" };
            const std::string body = "s0rt=" + wahid("var") + "&varid=" + wahid(varid) + " &appkey=" + wahid(ApplicationKey) + "&acckey=" + wahid(AccountKey);
            const auto response = request.send("POST", body, {
                {"Content-Type", "application/x-www-form-urlencoded"}
                });
            system("cls");
            if (sifr(std::string{ response.body.begin(), response.body.end() }) == "var_not_found")
            {
                return ">_<";
            }
            else if (sifr(std::string{ response.body.begin(), response.body.end() }) == "incorrect_application_details")
            {
                exit(0);
            }
            else
            {
                return sifr(std::string{ response.body.begin(), response.body.end() });
            }
        }
        catch (const std::exception& e)
        {
            exit(0);
        }
    }
};