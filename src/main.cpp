#include "window/app_window.h"

#include "third/json/json.h"

int main()
{
    Json json(R"({
        "list": [],
        "hash": {}
    })");

    json.SetList("list", 0);
    json.Set(0, "list", 0, 0);

    json.SetList("list", 1);
    json.Set(1, "list", 1, 0);

    json.SetList("list", 2);
    json.Set(2, "list", 2, 0);

    json.SetHash("hash", "0");
    json.Set("a", "hash", "0", "k");

    json.SetHash("hash", "1");
    json.Set("b", "hash", "1", "k");

    json.SetHash("hash", "2");
    json.Set("c", "hash", "2", "k");

    std::cout << json.ToString() << std::endl;

    std::cout << SFormat("{0}, {1}, {2}", 
                         json.Get("list", 0, 0).ToInt(), 
                         json.Get("list", 1, 0).ToInt(), 
                         json.Get("list", 2, 0).ToInt()) << std::endl;
    std::cout << SFormat("{0}, {1}, {2}",
                         json.Get("hash", "0", "k").ToString(),
                         json.Get("hash", "1", "k").ToString(),
                         json.Get("hash", "2", "k").ToString()) << std::endl;

    json.Get("list").ForList([&json](const size_t idx, const Json::Value & value)
                              {
                                 std::cout << SFormat("idx: {0}, val: {1}",
                                                      idx, json.Get(value, 0).ToInt()) << std::endl;
                              });
    json.Get("hash").ForHash([&json](const char * key, const Json::Value & value) 
                             {
                                 std::cout << SFormat("key: {0}, val: {1}", 
                                                      json.Get(value, "k").Key(), 
                                                      json.Get(value, "k").ToString()) << std::endl;
                             });
    std::cin.get();
    //json.Get("int").ToInt();
    //json.Get("float").ToFloat();
    //json.Get("true").ToBool();
    //json.Get("false").ToBool();
    //json.Get("list").ForList([](const Json::Value & value)
    //                          {
    //                              
    //                          });
    //json.Get("hash").ForHash([](const char * key, const Json::Value & value) {

    //                         });


    //auto json = std::move(Json::FromString("{}"));
    //json.Set(0, "int");
    //json.Set(1.0f, "float");
    //json.Set(true, "true");
    //json.Set(false, "false");
    //json.SetList("list");
    //json.Set("a", "list", 0);
    //json.Set("b", "list", 1);
    //json.Set("c", "list", 2);
    //json.SetHash("hash");
    //json.Set("a", "hash", "a");
    //json.Set("b", "hash", "b");
    //json.Set("c", "hash", "c");

    //json.Del(0, "0", "1");

    //Json::FromFile("");
    //Json::FromString("");

    //json.Get("1", "2", "3", "4", 0);
    //json.Del("1", "2", "3");
    //json.Set(xxx, "1", "2");
    //json.ToString();


	//AppWindow app;
	//app.Create("Render");
	//app.Move(100, 100);
	//app.Size(800, 800);
	//app.Loop();
	return 0;
}