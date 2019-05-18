# LICENSE

* **Author**: github.com/linqiongshan

* **本作品采用 <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议</a> 进行许可**

* ![CC Claim Picture](assets/by-nc-sa 4.0.png)

socket，socket 工具学习笔记

# 网络基础

# 编程接口：socket

# 网络 IO 模型

## select

## poll

## event poll

## 端口复用



# 工具/库

## Mongoose

* 轻量级 C/C++ HTTP 库

### 源码与架构分析

### 外部参考

#### 官方多线程使用示例

[Github Example Address: multithreaded](<https://github.com/cesanta/mongoose/blob/master/examples/multithreaded/multithreaded.c>)

```cpp
/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

static sig_atomic_t s_received_signal = 0;
static const char *s_http_port = "8000";
static const int s_num_worker_threads = 5;
static unsigned long s_next_id = 0;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}
static struct mg_serve_http_opts s_http_server_opts;
static sock_t sock[2];

// This info is passed to the worker thread
struct work_request {
  unsigned long conn_id;  // needed to identify the connection where to send the reply
  // optionally, more data that could be required by worker 
};

// This info is passed by the worker thread to mg_broadcast
struct work_result {
  unsigned long conn_id;
  int sleep_time;
};

static void on_work_complete(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev;
  char s[32];
  struct mg_connection *c;
  for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
    if (c->user_data != NULL) {
      struct work_result *res = (struct work_result *)ev_data;
      if ((unsigned long)c->user_data == res->conn_id) {
        sprintf(s, "conn_id:%lu sleep:%d", res->conn_id, res->sleep_time);
        mg_send_head(c, 200, strlen(s), "Content-Type: text/plain");
        mg_printf(c, "%s", s);
      }
    }
  }
}

void *worker_thread_proc(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  struct work_request req = {0};
  
  while (s_received_signal == 0) {
    if (read(sock[1], &req, sizeof(req)) < 0)
      perror("Reading worker sock");
    int r = rand() % 10;
    sleep(r);
    struct work_result res = {req.conn_id, r};
    mg_broadcast(mgr, on_work_complete, (void *)&res, sizeof(res));
  }
  return NULL;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;
  (void) ev_data;
  
  switch (ev) {
    case MG_EV_ACCEPT:
      nc->user_data = (void *)++s_next_id;
      break;
    case MG_EV_HTTP_REQUEST: {
      struct work_request req = {(unsigned long)nc->user_data};

      if (write(sock[0], &req, sizeof(req)) < 0)
        perror("Writing worker sock");
      break;
    }
    case MG_EV_CLOSE: {
      if (nc->user_data) nc->user_data = NULL;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;

  if (mg_socketpair(sock, SOCK_STREAM) == 0) {
    perror("Opening socket pair");
    exit(1);
  }

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "no";

  for (i = 0; i < s_num_worker_threads; i++) {
    mg_start_thread(worker_thread_proc, &mgr);
  }

  printf("Started on port %s\n", s_http_port);
  while (s_received_signal == 0) {
    mg_mgr_poll(&mgr, 200);
  }

  mg_mgr_free(&mgr);

  closesocket(sock[0]);
  closesocket(sock[1]);

  return 0;
}
```

#### 多线程使用分析（网络原始资料）

[原始地址](<https://blog.darkness463.top/2018/07/25/cpp-http-server-mongoose>)

最近有个奇葩的需求，需要用C++去实现一个Http Server。用Java或者其他语言去实现Http Server是一件很简单的事，但用C++去实现我就闻所未闻了（当然主要是我也没写过C++嘛）。于是去搜索了一把，不少人推荐[mongoose](https://github.com/cesanta/mongoose)，在实际使用中遇到了不少的坑。

##### 使用方式

使用mongoose只需要将mongoose.h和mongoose.c引入即可。下面通过几个简单的例子了解一下mongoose的使用及工作方式。

###### 简单的Web Server

这是官方给的[示例](https://github.com/cesanta/mongoose/tree/master/examples/simplest_web_server)。代码如下：

```
#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
```

其中有几个关键的结构体。
`mg_mgr`: 用于管理连接、事件等的Manager。
`mg_connection`: 单个连接，保存了连接信息。

从官方给的例子我们可以了解主要用法步骤：

1. 调用`mg_mgr_init`进行初始化。
2. 调用`mg_bind`，第2个参数为需要监听的端口号，第3个参数为处理请求的handler。
3. 调用`mg_set_protocol_http_websocket`将上一步返回的`mg_connection`与内建的http handler绑定。这样我们的handler才能收到http事件。
4. 通过一直调用`mg_mgr_poll`接收请求。

接下来我们看一下例子中的handler。

```
static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}
```



handler有3个参数，第1个参数是当前连接的`mg_connection`结构体，第2个参数是事件类型，第3个参数为事件的数据。

常用事件类型包含以下几种：

- `MG_EV_ACCEPT`: 新连接被接受。
- `MG_EV_HTTP_REQUEST`: 收到http请求，此时第3个参数可以将其强转为`http_message`结构体使用，包含了method、header、body等等信息。
- `MG_EV_CLOSE`: 连接关闭

例子中在收到`MG_EV_HTTP_REQUEST`事件时，调用了`mg_serve_http`，该方法会根据设置的根路径寻找html等资源。

###### RESTful Server

这个[例子](https://github.com/cesanta/mongoose/blob/master/examples/restful_server/restful_server.c)也是来自于官方。

```cpp
#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void handle_sum_call(struct mg_connection *nc, struct http_message *hm) {
  char n1[100], n2[100];
  double result;

  /* Get form variables */
  mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
  mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));

  /* Send headers */
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  /* Compute the result and send it back as a JSON object */
  result = strtod(n1, NULL) + strtod(n2, NULL);
  mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
  mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/api/v1/sum") == 0) {
        handle_sum_call(nc, hm); /* Handle RESTful call */
      } else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
        char buf[100] = {0};
        memcpy(buf, hm->body.p,
               sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
        printf("%s\n", buf);
      } else {
        mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
      }
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;
  int i;
  char *cp;
  const char *err_str;
#if MG_ENABLE_SSL
  const char *ssl_cert = NULL;
#endif

  mg_mgr_init(&mgr, NULL);

  /* Use current binary directory as document root */
  if (argc > 0 && ((cp = strrchr(argv[0], DIRSEP)) != NULL)) {
    *cp = '\0';
    s_http_server_opts.document_root = argv[0];
  }

  /* Process command line options to customize HTTP server */
  // 省略这部分代码

  /* Set HTTP server options */
  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.error_string = &err_str;
#if MG_ENABLE_SSL
  if (ssl_cert != NULL) {
    bind_opts.ssl_cert = ssl_cert;
  }
#endif
  nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
  if (nc == NULL) {
    fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
            *bind_opts.error_string);
    exit(1);
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.enable_directory_listing = "yes";

  printf("Starting RESTful server on port %s, serving %s\n", s_http_port,
         s_http_server_opts.document_root);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
```

主要区别在于处理`MG_EV_HTTP_REQUEST`时，根据请求的uri的不同进行不同的处理。示例中请求`/api/v1/sum`时，调用了`handle_sum_call`进行处理。

从示例的`handle_sum_call`方法我们可以了解返回结果基本步骤。

1. 根据业务逻辑得到需要返回的字符串。
2. 调用`mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");`回写headers。
3. 调用`mg_printf_http_chunk(nc, result);`回写response，其中`result`即第1步得到的要返回的字符串。
4. 调用`mg_send_http_chunk(nc, "", 0);`作为结束。

###### 请求信息的获取

处理具体业务时，我们可能需要根据请求的header、method、body等进行相应的处理。前文提到我们在收到`MG_EV_HTTP_REQUEST`类型的回调时，可以将第3个参数转为`http_message`结构体，其定义如下，可以结合官方的注释了解其含义。

```
struct http_message {
    struct mg_str message; /* Whole message: request line + headers + body */
    struct mg_str body;    /* Message body. 0-length for requests with no body */

    /* HTTP Request line (or HTTP response line) */
    struct mg_str method; /* "GET" */
    struct mg_str uri;    /* "/my_file.html" */
    struct mg_str proto;  /* "HTTP/1.1" -- for both request and response */

    /* For responses, code and response status message are set */
    int resp_code;
    struct mg_str resp_status_msg;

    /*
     * Query-string part of the URI. For example, for HTTP request
     *    GET /foo/bar?param1=val1&param2=val2
     *    |    uri    |     query_string     |
     *
     * Note that question mark character doesn't belong neither to the uri,
     * nor to the query_string
     */
    struct mg_str query_string;

    /* Headers */
    struct mg_str header_names[MG_MAX_HTTP_HEADERS];
    struct mg_str header_values[MG_MAX_HTTP_HEADERS];
};
```

其中每一个属性都是`mg_str`结构体，其定义如下。

```
struct mg_str {
    const char *p; /* Memory chunk pointer */
    size_t len;    /* Memory chunk length */
};
```

可以看出，`mg_str`实际就是存了起始地址和长度，所有我们从相应地址开始截取指定长度就是对于的属性内容。我们可以用类似下面这种方式获取相应属性：

```
std::string uri = std::string(http_req->uri.p, http_req->uri.len);
std::string body = std::string(http_req->body.p, http_req->body.len);
std::string method = std::string(http_req->method.p, http_req->method.len);
```

headers的获取类似，header的key和value分别存放在`header_names`和`header_values`2个数组中，数组的长度`MG_MAX_HTTP_HEADERS`默认值为40，因此最大只支持40个header，当然我们可以自行修改该值。

##### 多线程

前文已经介绍了mongoose的简单使用了，但都是单线程的方式，QPS必然很难上去。mongoose也能支持1个IO线程、N个工作线程的模式。我们还是先来看一下[官方示例](https://github.com/cesanta/mongoose/blob/master/examples/multithreaded)。

```cpp
#include "mongoose.h"

static sig_atomic_t s_received_signal = 0;
static const char *s_http_port = "8000";
static const int s_num_worker_threads = 5;
static unsigned long s_next_id = 0;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}
static struct mg_serve_http_opts s_http_server_opts;
static sock_t sock[2];

// This info is passed to the worker thread
// 用于IO线程向工作线程发送请求
struct work_request {
  unsigned long conn_id;  // needed to identify the connection where to send the reply
  // optionally, more data that could be required by worker 
};

// This info is passed by the worker thread to mg_broadcast
// 用于工作线程向IO线程返回结果
struct work_result {
  unsigned long conn_id;
  int sleep_time;
};

// 运行在IO线程，用于回写结果
static void on_work_complete(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev;
  char s[32];
  struct mg_connection *c;
  for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
    if (c->user_data != NULL) {
      struct work_result *res = (struct work_result *)ev_data;
      if ((unsigned long)c->user_data == res->conn_id) {
        sprintf(s, "conn_id:%lu sleep:%d", res->conn_id, res->sleep_time);
        mg_send_head(c, 200, strlen(s), "Content-Type: text/plain");
        mg_printf(c, "%s", s);
      }
    }
  }
}

// 工作线程
void *worker_thread_proc(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  struct work_request req = {0};
  
  while (s_received_signal == 0) {
    if (read(sock[1], &req, sizeof(req)) < 0)
      perror("Reading worker sock");
    int r = rand() % 10;
    sleep(r);
    struct work_result res = {req.conn_id, r};
    mg_broadcast(mgr, on_work_complete, (void *)&res, sizeof(res));
  }
  return NULL;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;
  (void) ev_data;
  
  switch (ev) {
    case MG_EV_ACCEPT:
    // 给连接设置一个id，用于后面IO线程回写结果找到对应的id回写。
      nc->user_data = (void *)++s_next_id;
      break;
    case MG_EV_HTTP_REQUEST: {
      struct work_request req = {(unsigned long)nc->user_data};

      if (write(sock[0], &req, sizeof(req)) < 0)
        perror("Writing worker sock");
      break;
    }
    case MG_EV_CLOSE: {
      if (nc->user_data) nc->user_data = NULL;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;

// 起一对socket，用于IO线程向工作线程发送请求
  if (mg_socketpair(sock, SOCK_STREAM) == 0) {
    perror("Opening socket pair");
    exit(1);
  }

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "no";

  for (i = 0; i < s_num_worker_threads; i++) {
    mg_start_thread(worker_thread_proc, &mgr);
  }

  printf("Started on port %s\n", s_http_port);
  while (s_received_signal == 0) {
    mg_mgr_poll(&mgr, 200);
  }

  mg_mgr_free(&mgr);

  closesocket(sock[0]);
  closesocket(sock[1]);

  return 0;
}
```

简单来说，流程是IO线程收到请求后，将请求通过socket发往工作线程，工作线程处理完后，通过`mg_broadcast`方法返回IO线程，mongoose内部也有一对socket用于工作线程向IO线程通信，IO线程收到后向对应的连接回写结果。

我们一个一个关键点来看。

###### socket

```
if (mg_socketpair(sock, SOCK_STREAM) == 0) {
    perror("Opening socket pair");
    exit(1);
}
```

通过调用`mg_socketpair`方法开启一对socket，如前文所述，这对socket是用于IO线程向工作线程发送请求的。而工作线程向IO线程返回结果的socket不需要我们操心，mongoose帮我们实现了。
示例中定义了`work_request`和`work_result`2个结构体用于传输请求和结果，我们可以根据业务需要修改这2个结构体，但需要注意序列化的问题。

另外这里有个隐藏的坑。mongoose帮我们实现的socket利用了其定义的`ctl_msg`结构体进行传输。其定义如下：

```
struct ctl_msg {
    mg_event_handler_t callback;
    char message[MG_CTL_MSG_MESSAGE_SIZE];
};
```

其中`MG_CTL_MSG_MESSAGE_SIZE`默认值是8192，因此如果你的返回结果超过8k会有问题，需要将该值调大。

如果不想用socket从IO线程向工作线程发请求的话，我们也可以使用队列之类的，IO线程将请求往队列扔，工作线程一直从队列读请求。这里就不写具体实现了。

###### 工作线程

```
for (i = 0; i < s_num_worker_threads; i++) {
    mg_start_thread(worker_thread_proc, &mgr);
}
```

示例中这部分代码即开启工作线程，我们可以根据自己的需要启动任意数量的工作线程。需要注意的是我们要将`MG_ENABLE_THREADS`的值改为1，否则`mg_start_thread`是无法调用的。

示例中的工作线程的实现如下：

```
void *worker_thread_proc(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  struct work_request req = {0};
  
  while (s_received_signal == 0) {
    if (read(sock[1], &req, sizeof(req)) < 0)
      perror("Reading worker sock");
    int r = rand() % 10;
    sleep(r);
    struct work_result res = {req.conn_id, r};
    mg_broadcast(mgr, on_work_complete, (void *)&res, sizeof(res));
  }
  return NULL;
}
```

首先通过`read`把`work_request`读出来，处理完后调用`mg_broadcast`将`work_result`返回IO线程，其中`on_work_complete`是返回IO线程后需要执行的方法。上节已经提到了，需要注意一下返回长度的问题。

IO线程（接收请求）

```
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;
  (void) ev_data;
  
  switch (ev) {
    case MG_EV_ACCEPT:
      nc->user_data = (void *)++s_next_id;
      break;
    case MG_EV_HTTP_REQUEST: {
      struct work_request req = {(unsigned long)nc->user_data};

      if (write(sock[0], &req, sizeof(req)) < 0)
        perror("Writing worker sock");
      break;
    }
    case MG_EV_CLOSE: {
      if (nc->user_data) nc->user_data = NULL;
    }
  }
}
```

在`MG_EV_ACCEPT`事件时，为`mg_connection`的`user_data`赋一个id，之后需要根据这个id确定向哪个连接回写结果。`MG_EV_CLOSE`事件时将该id置空。

而在`MG_EV_HTTP_REQUEST`时，通过`write`方法将封装的`work_request`向socket写。这样便能被上一节中的工作线程从socket中读到。

###### IO线程（返回结果）

```
static void on_work_complete(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev;
  char s[32];
  struct mg_connection *c;
  for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
    if (c->user_data != NULL) {
      struct work_result *res = (struct work_result *)ev_data;
      if ((unsigned long)c->user_data == res->conn_id) {
        sprintf(s, "conn_id:%lu sleep:%d", res->conn_id, res->sleep_time);
        mg_send_head(c, 200, strlen(s), "Content-Type: text/plain");
        mg_printf(c, "%s", s);
      }
    }
  }
}
```

这里的逻辑是遍历所有的连接，找出id相同的连接（即上一节提到的`MG_EV_ACCEPT`事件时设置的id），然后向改连接回写结果。

这里示例的实现我觉得有点问题，我们看一下mongoose中是如何回调`on_work_complete`方法的。前文提到mongoose中也有一对socket，用于工作线程向IO线程发送结果，结果是在`mg_mgr_handle_ctl_sock`方法中取出来的，代码如下：

```
static void mg_mgr_handle_ctl_sock(struct mg_mgr *mgr) {
    struct ctl_msg ctl_msg;
    int len =
            (int) MG_RECV_FUNC(mgr->ctl[1], (char *) &ctl_msg, sizeof(ctl_msg), 0);
    size_t dummy = MG_SEND_FUNC(mgr->ctl[1], ctl_msg.message, 1, 0);
    DBG(("read %d from ctl socket", len));
    (void) dummy; /* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=25509 */
    if (len >= (int) sizeof(ctl_msg.callback) && ctl_msg.callback != NULL) {
        struct mg_connection *nc;
        for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
            ctl_msg.callback(nc, MG_EV_POLL,
                             ctl_msg.message MG_UD_ARG(nc->user_data));
        }
    }
}
```

可以清楚地看到，mongoose已经将所有的连接遍历一次了，并且会将每个连接当参数执行回调的方法（即`ctl_msg.callback(nc, MG_EV_POLL, ctl_msg.message MG_UD_ARG(nc->user_data));`）。所有我理解在`on_work_complete`中我们没必要再遍历一次了，只需要判断传过来的`mg_connection`是不是我们需要的那个连接，对正确的那个连接回写即可。

##### 性能

在16核 16G内存的机器上进行了一下压测。同样都以20个并发压（恕我直言，并发太高这个框架根本扛不住）。压了3个不同的版本，这三个版本逻辑基本是一致的。

```
版本一： Go + C++版本。Go实现Http Server，Thrift调用C++逻辑代码。
版本二： C++版本。C++实现Http Server（基于mongoose），逻辑部分和版本一一样，因为整合成了一个程序，所以直接调用逻辑部分的方法。
版本三： Java版本。基于Spring Boot。
```

在20的并发下，版本一最大QPS 3500左右，不过此时平均耗时已经增加到了30~40ms。版本二最大QPS 2000左右，平均耗时6ms。版本三最大QPS 5000~5500，平均耗时4ms。

##### 总结

1. mongoose并不算很成熟，性能上也不怎么样。
2. 如果真的需要使用，最好再进行一层封装。可以参考[这位同学的实现](https://github.com/tashaxing/CppHttpDemo)。
3. C++用于应用层不仅开发起来费时，得到的好处似乎也有限，可能是我太年轻？

> 最后更新时间：2018-12-06 10:50:40 
> 原始链接：[http://blog.darkness463.top/2018/07/25/cpp-http-server-mongoose/](https://blog.darkness463.top/2018/07/25/cpp-http-server-mongoose/)


