#include <SFML/Network.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <malloc.h>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#pragma warning(disable : 4700).
#pragma warning(disable : 4996)
using namespace std;
using namespace sf;
struct HTTPrequest {
  string method;
  string path;
  string version;

  string accept;
  string accept_language;
  string accept_encoding;
  string user_agent;

  string status;

  string auth_realm;

  string answer;

  bool authentication_given;
  string username;
  string password;
  map<string, string> params;
};
string ReadFiles(string place) {
  string data = "";
  ifstream file;
  if (place.find("jpg") != string::npos or place.find("ico") != string::npos or
      place.find("png") != string::npos) {
    file.open(place, ios_base::binary);
    // file.open(place);
    cout << "Binary file " << place << endl;
  } else {
    file.open(place);
  }
  if (file.is_open()) {
    // while (!file.eof())
    //{
    file.seekg(0, ios_base::end);
    size_t size = file.tellg();
    cout << "Size is " << size << endl;
    char *buf = new char[size + 1];
    file.seekg(0, ios_base::beg);
    file.read(buf, size);
    buf[size] = 0;
    data = buf;
    delete[] buf;
    //}
  } else {
    data = "Failed to load file from DB";
  }
  file.close();
  return data;
}
vector<char> ReadFiles2(string place, int &s) {
  ifstream file;
  file.open(place, ios::binary);
  if (file.is_open()) {
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    cout << "Size is " << size << endl;
    vector<char> buf(size + 1, ' ');
    file.seekg(0, ios::beg);
    file.read(buf.data(), size);
    /*if (place.find(".html") == string::npos
        and place.find(".css") == string::npos) {
        buf.push_back(0);
    }*/
    buf[size] = '\0';
    s = size;
    file.close();
    return buf;
    // delete[]buf;
    // }
  } else {
    cout << "Failed to load file from DB" << endl;
    vector<char> buf(1, '\0');
    return buf;
  }

  // return buf;
}
void ReadFiles3(string place, int &s, char *buf) {
  ifstream file;
  file.open(place, ios_base::binary);
  if (file.is_open()) {
    file.seekg(0, ios_base::end);
    size_t size = file.tellg();
    cout << "Size is " << size << endl;
    buf = new char[size + 1];
    file.seekg(0, ios_base::beg);
    file.read(buf, size);
    buf[size] = 0;
    s = size;
    file.seekg(0, ios_base::beg);
    file.close();
    // delete[]buf;
    // }
  } else {
    cout << "Failed to load file from DB" << endl;
  }

  // return buf;
}
void SplitGetReq(string get_req, string &path, map<string, string> &params) {
  // Remove trailing newlines
  if (get_req[get_req.size() - 1] == '\x0d' ||
      get_req[get_req.size() - 1] == '\x0a')
    get_req = get_req.substr(0, get_req.size() - 1);

  if (get_req[get_req.size() - 1] == '\x0d' ||
      get_req[get_req.size() - 1] == '\x0a')
    get_req = get_req.substr(0, get_req.size() - 1);

  // Remove potential Trailing HTTP/1.x
  if (get_req.size() > 7)
    if (get_req.substr(get_req.size() - 8, 7) == "HTTP/1.")
      get_req = get_req.substr(0, get_req.size() - 9);

  string::size_type qm = get_req.find("?");
  if (qm != string::npos) {
    string url_params = get_req.substr(qm + 1);

    path = get_req.substr(0, qm);

    // Appending a '&' so that there are as many '&' as name-value pairs.
    // It makes it easier to split the url for name value pairs, he he he
    url_params += "&";

    string::size_type next_amp = url_params.find("&");

    while (next_amp != std::string::npos) {
      string name_value = url_params.substr(0, next_amp);
      url_params = url_params.substr(next_amp + 1);
      next_amp = url_params.find("&");

      string::size_type posocketequal = name_value.find("=");

      string nam = name_value.substr(0, posocketequal);
      string val = name_value.substr(posocketequal + 1);

      std::string::size_type posocketplus;
      while ((posocketplus = val.find("+")) != string::npos)
        val.replace(posocketplus, 1, " ");

      // Replacing %xy notation
      string::size_type posockethex = 0;
      while ((posockethex = val.find("%", posockethex)) != string::npos) {
        stringstream h;
        h << val.substr(posockethex + 1, 2);
        h << hex;

        int i;
        h >> i;

        stringstream f;
        f << static_cast<char>(i);
        std::string s;
        f >> s;

        val.replace(posockethex, 3, s);
        posockethex++;
      }

      params.insert(map<string, string>::value_type(nam, val));
    }
  } else
    path = get_req;
}
string Process(HTTPrequest req) {
  string answer;
  if (req.path.find("jpg") != string::npos) {
    answer += "Content-Type: image/jpg\r\n";
  } else if (req.path.find("png") != string::npos) {
    answer += "Content-Type: image/png\r\n";
  } else if (req.path.find("ico") != string::npos) {
    answer += "Content-Type: image/ico\r\n";
  } else if (req.path.find("css") != string::npos) {
    answer += "Content-Type: text/css\r\n";
  } else if (req.path.find("html") != string::npos or
             req.path.find("html") == string::npos) {
    answer += "Content-Type: text/html\r\n";
  }
  return answer;
}
int main() {
  setlocale(LC_ALL, "Russian");
  TcpListener listener;
  if (listener.listen(7860) != Socket::Done) {
    cout << "Unable to listen to the port 15000" << endl;
    listener.listen(listener.AnyPort);
    cout << "New binding pori is: " << listener.getLocalPort() << endl;
    // system("pause");
    // return 0;
  }
  SocketSelector selector;
  list<TcpSocket *> clients;
  selector.add(listener);
  // int s;
  // vector<char> ans = ReadFiles2("GID/index.html", s);
  // cout << ans.data() << endl;
  start:
  while (true) {
    if (selector.wait()) {
      if (selector.isReady(listener)) {
        TcpSocket *client = new TcpSocket;
        if (listener.accept(*client) == Socket::Done) {
          clients.push_back(client);
          selector.add(*client);
          // cout << "New client" << endl;
          // cout << client->getRemoteAddress() << " " << endl;
        } else {
          delete client;
        }
      } else {
        list<TcpSocket *>::iterator it = clients.begin();
        TcpSocket &client = **it;
        if (selector.isReady(client)) {
          HTTPrequest req;
          char buffer[1024 * 10];
          size_t size = 0;
          string ReceivedStr;
          // while(selector.isReady(client)){
          if (client.receive(buffer, sizeof(buffer), size) == Socket::Done) {
            ReceivedStr.append(buffer, buffer + size);
          }
          cout << ReceivedStr << endl;
          if (ReceivedStr.empty()) {
            selector.remove(client);
            client.disconnect();
            clients.erase(it);
          }
          if (ReceivedStr.find("GET") == 0) {
            req.method = "GET";
          } else if (ReceivedStr.find("POST") == 0) {
            req.method = "POST";
          }
          req.status = "202 OK";
          string path;
          map<string, string> params;
          size_t posStartPath = ReceivedStr.find_first_not_of(" ", 3);
          size_t posEndPath = ReceivedStr.find_first_of("\n\r", 3);
          if (posStartPath == std::string::npos || posEndPath == std::string::npos || posStartPath >= posEndPath)
          {
            cout << "Invalid request format" << endl;
            goto start;
          }

          SplitGetReq(
              ReceivedStr.substr(posStartPath, posEndPath - posStartPath), path,
              params);
          req.path = path;
          req.params = params;
          // cout << ReceivedStr << endl;

          time_t localtime;
          time(&localtime);
          tm *gmt = gmtime(&localtime);
          char *rtime = asctime(gmt);
          rtime[24] = 0;

          int s = 0;
          if (path == "/") {
            path += "index.html";
          }
          cout << path << endl;
          vector<char> answer = ReadFiles2("Galina" + path, s);
          s = answer.size();
          /*char* answer;
          ReadFiles3("GID" + path, s, answer);*/

          string Response = "HTTP/1.1 ";
          Response += req.status;
          Response += "\r\n";
          Response += "Date: ";
          Response += rtime;
          Response += "\r\n";
          Response += "Server: SSS_Krut's server\r\n";
          Response += "Connection: close\r\n";
          Response += Process(req);
          Response += "Content-Length: ";
          Response += to_string(s);
          cout << "Real size: " << s << endl;
          // answer = ReadFiles("GID/index.html");
          Response += "\r\n\r\n";
          // Response += answer.data();
          for (int i = 0; i != s; i++) {
            Response += answer[i];
          }

          // for (int i = 0; i != s; i++) {
          //     Response += answer[i];
          //     cout << answer[i];
          //     /*if (path.find("html") != string::npos) {
          //         cout << answer[i];
          //     }*/
          // }
          // delete answer;
          // Response += "\r\n\r\n";

          /* answer = ReadFiles("GID/all.css");
           Response += "\r\n\r\n";
           Response += answer;*/

          /*Response += "--Asrf456BGe4h";
          Response += "Content-Type: image/jpg\r\n";
          answer = ReadFiles("GID/images/original-1.jpg");
          Response += answer;*/
          // answer = ReadFiles("GID/images/original-1.jpg");
          // Response += answer;
          // answer = ReadFiles("GID/images/original-1.jpg");
          // Response += "Content-Length: ";
          // Response += to_string(answer.size());
          // Response += "\r\n\r\n";
          // Response += answer;
          if (client.send(Response.c_str(), Response.size()) != Socket::Done) {
            cout << "Something went wrong with " << client.getRemoteAddress()
                 << endl;
          }
          // answer = ReadFiles("GID/images/original-1.jpg");
          // Response = "HTTP/1.1 ";
          // Response += req.status;
          // Response += "\r\n";
          // Response += "Date: ";
          // Response += rtime;
          // Response += "\r\n";
          // Response += "Server: SSS_Krut's server\r\n";
          // Response += "Connection: close\r\n";
          ////Response += "Content-Type: text/html; charset=UTF-8 \r\n";
          // Response += "Content-Type: image/png\r\n";
          // Response += "Content-Length: ";
          // Response += to_string(answer.size());
          // Response += "\r\n\r\n";
          // Response += answer;
          // if (client.send(Response.c_str(), Response.size()) != Socket::Done)
          // {
          //     cout << "Something went wrong with " <<
          //     client.getRemoteAddress() << endl;
          // }
          // }
          cout << endl;
          selector.remove(client);
          client.disconnect();
          clients.erase(it);
        }
      }
    }
  }
  return 0;
}
