# 7-Day Advanced Networking Curriculum: From OSI to WebSockets

**Overview:**
This one-week intensive curriculum will take you on a deep dive through the network stack – from low-level TCP/IP fundamentals up to modern web protocols like HTTP/2 and WebSockets. Each day is structured by network layers, combining theoretical foundations (with RFC references and standards) and practical hands-on exercises (primarily in Rust). By the end, you’ll have built components of a networking stack and web server from scratch, inspected real traffic on the wire, and gained a robust understanding of *“what happens under the hood”* of web applications.

**Curriculum structure:**

* **Day 1 – Link & Network Layers:** OSI model, Ethernet/IPv4/IPv6, ARP, ICMP.
* **Day 2 – Transport Layer (TCP/UDP):** TCP/IP mechanics, UDP differences, sockets.
* **Day 3 – Application Layer (HTTP/1.1):** HTTP fundamentals, requests/responses.
* **Day 4 – Security Layer (TLS & HTTPS):** TLS handshake, certificates, HTTPS.
* **Day 5 – Application Layer (HTTP/2):** Binary framing, multiplexing, header compression.
* **Day 6 – Application Layer (WebSockets):** Upgrade handshake, full-duplex communication.
* **Day 7 – Integration & Advanced Topics:** Build a mini web server combining HTTP/1.1 + TLS + WebSockets, explore real-world performance and next steps.

Before you begin, ensure you have the following tools installed: **Wireshark**, **tcpdump**, **curl**, **openssl**, **Rust & Cargo** (with `rustc` and `cargo`), and a preferred IDE. We’ll also use various Rust crates (like `tokio`, `mio`, `hyper`, `actix-web`, `rustls`, `tungstenite`) throughout the week.

---

## Day 1: OSI Model, Ethernet, and IP Fundamentals (Link & Network Layers)

**Goals:** Gain a solid understanding of the OSI 7-layer model and the Internet (TCP/IP) model. Learn how data is encapsulated from higher to lower layers and travels across a network. Study the link layer (Ethernet framing, MAC addressing, ARP) and network layer (IPv4/IPv6 addressing, routing). Practice inspecting raw network frames and packets on the wire.

**Key Topics & Concepts:**

* **OSI vs. TCP/IP Models:** Understand each of the 7 OSI layers and how the 5-layer Internet model maps to it. Focus on layers 1-4 today (Physical, Data Link, Network, Transport). Recognize that modern Internet protocols use a simpler TCP/IP stack (link, internet, transport, application).

* **Ethernet & MAC:** Basics of Ethernet (Layer 2) framing. MAC addresses and how local network communication works. Learn about switches vs. hubs, and how an Ethernet frame is structured (preamble, source/dest MAC, EtherType, payload, CRC).

* **ARP (Address Resolution Protocol):** How IP addresses map to MAC on a LAN. Understand ARP requests/replies (and the ARP cache). This is a protocol operating *between* Link and Network layers to bridge layer-3 to layer-2 addressing.

* **IP (Internet Protocol):** IPv4 packet structure (version, header length, TTL, protocol, source & dest addresses, etc.). Concept of fragmentation and reassembly (and why modern networks avoid it with MTU Path Discovery). Briefly compare IPv4 and IPv6 (128-bit addresses, no fragmentation by routers, simplified header in IPv6). Know that IP is *unreliable* and connectionless – higher layers must handle reliability.

* **ICMP:** Internet Control Message Protocol, used for network diagnostics (e.g. `ping` uses ICMP Echo Request/Reply). Understand that ICMP is layer-3 (network-layer) and learn how tools like **ping** and **traceroute** work (ICMP Echo and TTL expiry). ICMP messages have their own type/code and are carried within IP packets.

* **Encapsulation:** Each layer wraps the data from above in its own header (and trailer at link layer). E.g., an HTTP request (L7) goes into a TCP segment (L4), which goes into an IP packet (L3), which goes into an Ethernet frame (L2) on the wire. Learn to visualize this nesting.

**Recommended Reading:**

* *OSI Model:* Imperva’s “OSI Model Explained” for a high-level refresher. Focus on the role of each layer and how responsibilities are divided (e.g. routing at network layer, reliability at transport, etc.).
* *IPv4:* RFC 791 (Internet Protocol v4) sections 1-3 for an overview of the IP header fields. (For IPv6, see RFC 8200). Identify fields like TTL, Protocol (which indicates whether it’s TCP, UDP, ICMP in payload), and how fragmentation is handled.
* *ICMP:* RFC 792 for ICMP message types (Echo Request/Reply, Destination Unreachable, Time Exceeded used by traceroute, etc.).
* *Networking Textbook:* **Computer Networks: A Top-Down Approach** (Kurose & Ross) Ch.1.5 and Ch.4 (coverage of link and network layers) – if available, skim for conceptual clarity on how data moves through layers.
* *Rust & Network Programming:* Review the Rust `std::net` documentation (for low-level sockets) and the `packet` crate like [`pnet`](https://crates.io/crates/pnet) if you plan to craft packets manually.

**Hands-On Exercises:**

* **1. Visualize OSI Layers:** Draw a diagram of the 7-layer OSI model and list example protocols at each layer (e.g. Ethernet at layer 2, IP at layer 3, TCP at layer 4, HTTP at layer 7, etc.). This will be your mental map for the week.

* **2. Packet Capture Basics:** Use **Wireshark** to capture traffic on your local machine. Apply a filter for ICMP. Run a `ping` to some host (e.g. `ping 8.8.8.8`) and observe the captured **ICMP Echo Request** and **Echo Reply** packets. Inspect the Ethernet frame, IP packet, and ICMP message within. Wireshark allows you to expand each layer – identify the fields (e.g., IP source/dest, TTL, ICMP type/code) and note how they correspond to the theory. *Tip:* In Wireshark, right-click a packet and select “Follow > UDP/TCP Stream” or use the Hex/ASCII view to see raw bytes.

* **3. ARP in Action:** On your local network, use `arp -a` (Windows/Linux) to see the ARP cache. Clear it if possible, then ping a local IP and watch **arp** requests in Wireshark (filter: `arp`). You should see the ARP query “Who has IP x.x.x.x?” and the response with the MAC address. This demonstrates how an IP address is resolved to a link-layer address.

* **4. Raw Socket Programming (Optional, Advanced):** Try sending an ICMP Echo (ping) using a raw socket in Rust. This requires setting up a raw socket (which may need admin privileges). You can use the [`socket2`](https://docs.rs/socket2) crate or the lower-level libc to create a raw socket for IPPROTO\_ICMP. Construct an ICMP Echo Request packet (Type 8, Code 0) with proper checksum. (Refer to the ICMP RFC or use an existing crate for packet construction.) Send the packet to a target and receive the response. **Note:** This is low-level and tricky due to needing correct byte order and checksums – it’s a great exercise to appreciate what the OS does for you. If stuck, refer to community examples like the “Implementing ICMP in Rust” blog, which shows an ICMP packet captured and its hex dump.

* **5. Tools Practice:** Familiarize yourself with **tcpdump** as a quick CLI alternative to Wireshark. For example, run:

  ```bash
  sudo tcpdump -i any -c 5 -vvv -X icmp  
  ```

  This captures 5 ICMP packets with verbose output and hex (`-X`). Notice the same info: you’ll see IP header fields (identified by tcpdump) and the payload in hex (which for ping contains a timestamp or arbitrary data). Learning to read tcpdump is valuable for remote server debugging where you may not have a GUI.

By the end of Day 1, you should be comfortable with the basics of how data is packaged at layers 2 and 3, and you’ll have hands-on experience capturing and interpreting raw packets on the wire. You’ll also have your environment set up with essential tools for deeper exploration in upcoming days.

---

## Day 2: Transport Layer Deep Dive – TCP (and UDP) Internals

**Goals:** Master the transport layer, focusing on **TCP** – its connection mechanisms, reliability features, and real-world behavior. Understand UDP’s simplicity by contrast. Learn how sockets work at the OS level and how to use Rust to create socket servers/clients. Explore TCP’s 3-way handshake, flow control, and congestion control, and inspect these in practice with tools.

**Key Topics & Concepts:**

* **TCP vs UDP Overview:** TCP (Transmission Control Protocol) provides reliable, ordered, error-checked delivery of a stream of bytes. UDP (User Datagram Protocol) is connectionless, sending individual packets (“datagrams”) with no built-in reliability. Key differences: TCP is heavier but guarantees delivery and order; UDP is lightweight but may lose or reorder packets.

* **TCP 3-Way Handshake:** The process of establishing a TCP connection: *SYN*, *SYN-ACK*, *ACK*. The client initiates with a SYN (synchronize) packet (with an initial sequence number), server responds with SYN-ACK (synchronizing and acknowledging client’s SYN), and client sends final ACK. After this, both sides have agreed on sequence numbers and the connection is established. Likewise, connection termination involves a 4-step FIN/ACK (each side closes independently) plus the TIME-WAIT state.

  &#x20;*Illustration of the TCP three-way handshake (client on left, server on right). The blue arrows indicate the SYN and SYN-ACK segments, and the green arrow the final ACK, establishing a TCP connection.*

* **TCP Reliability Features:** TCP ensures *ordered data transfer* and *retransmission of lost packets*, provides *error detection* via checksums, implements *flow control* (using a sliding window to avoid overwhelming the receiver), and employs *congestion control* algorithms to be fair and avoid network meltdown. Study how TCP sequence and acknowledgment numbers work to track bytes delivered. Understand the concept of the **sliding window** and how the receiver’s *advertised window* (rwnd) and TCP’s congestion window (cwnd) limit how much data can be in flight.

* **TCP Flow & Congestion Control:** Learn about **slow start**, **additive increase/multiplicative decrease**, and algorithms like Reno, CUBIC, etc., at a high level. You don’t need to implement them, but know that TCP will ramp up sending rate until loss is detected, then back off. Realize that these algorithms are why TCP traffic will adjust to network conditions.

* **UDP Characteristics:** No handshake, no guaranteed delivery or order. Useful for simple query/response protocols (DNS) or real-time applications (VoIP, gaming) where dropping some packets is acceptable. Understand when you’d choose UDP and that if reliability is needed over UDP, it must be handled at the application layer (or use an existing protocol like QUIC which builds reliability on UDP).

* **Sockets API:** The programming interface for transport layer. Concepts: **listening socket** vs **connected socket**, binding to a port, client vs server roles. The lifecycle: `socket() -> bind() -> listen() -> accept()` (server side) and `socket() -> connect()` (client). Understand the meaning of a *port*. Learn how to use `netstat`/`ss` to view TCP socket states (LISTEN, ESTABLISHED, TIME\_WAIT, etc.). You’ll map these states to the handshake and termination steps.

* **Rust Networking:** Use Rust’s standard library (`std::net::TcpListener`, `TcpStream`, `UdpSocket`) for basic socket operations. Additionally, understand how **mio** (a low-level OS event library) and **Tokio** (an async runtime) allow handling multiple connections efficiently (via non-blocking I/O and event loops). Tokio builds on mio; you might experiment with both to see the difference between manual polling and the ergonomics of async/await.

**Recommended Reading:**

* *TCP Specification:* RFC 9293 (Aug 2022) which **obsoletes RFC 793** as the updated TCP spec. Read Section 3 (Functional Specification) for a description of connection state machine and data transfer. Pay attention to the state diagram of TCP (LISTEN, SYN-SENT, ESTABLISHED, FIN-WAIT-1, etc.). Also skim RFC 793’s original section on the 3-way handshake for a narrative explanation (even though 793 is old, the basics are the same; RFC 9293 consolidates updates).
* *TCP Handshake & States:* “TCP handshake” on MDN Web Docs or in any networking textbook will reinforce the SYN, SYN-ACK, ACK sequence and show a state transition diagram. Ensure you understand why TIME\_WAIT exists (to absorb late packets).
* *Flow Control & Congestion:* Chapters 3 and 6 of **“High Performance Browser Networking”** by Ilya Grigorik are excellent – Chapter *“Building Blocks of TCP”* explains TCP flow control, congestion control (slow start etc.), with diagrams. This will give you insight into how latency and throughput are related (bandwidth-delay product) and issues like head-of-line blocking in TCP.
* *Linux Network Tuning:* If interested in real-world, read about the TCP *socket options* and Linux `/proc/sys/net/ipv4` settings (e.g., `tcp_rmem`, `tcp_congestion_control`). (This is optional and for context of how OS allows tuning TCP behavior.)
* *Rust & Async:* Read the [Tokio tutorial](https://tokio.rs/tokio/tutorial) sections on writing a simple echo server. Also, skim the `mio` crate docs to see how to manually register sockets with the event loop (you might not use mio directly in final projects, but it’s good to know what’s under the hood of Tokio).

**Hands-On Exercises:**

* **1. Capture a TCP Handshake:** In Wireshark, capture traffic and filter by a specific host or port. For example, run a simple command like `curl http://example.com` (which will use TCP port 80). In Wireshark, filter by `tcp.port == 80`. Find the **SYN**, **SYN-ACK**, **ACK** packets of the handshake. Wireshark will label them “\[SYN]”, “\[SYN, ACK]”, etc. Examine the TCP header in each: note the *sequence number* and *ack number*. In the SYN, the client’s seq is `x`; in the SYN-ACK, server seq is `y` and ack = `x+1`; in the final ACK, ack = `y+1`. This exercise makes the abstract handshake concrete. (You can also use **tcpdump**: `tcpdump -n -v "tcp[13] == 2"` filters SYN packets, etc., but Wireshark is easier.)

* **2. Socket Programming – TCP Echo Server:** Time to write some Rust! Using `std::net::TcpListener`, create a basic **Echo Server** that listens on a port (say 8080) and sends back any data it receives. Pseudocode:

  ```rust
  let listener = TcpListener::bind("0.0.0.0:8080")?;  
  for stream in listener.incoming() {  
      let mut stream = stream?;  
      std::thread::spawn(move || {  
          // read from stream into a buffer  
          // write the same data back to stream  
      });  
  }  
  ```

  Test your echo server using `nc` (netcat): in one terminal run `nc localhost 8080`, type something and press enter – you should see the same text echoed back. This exercise teaches accept loops, basic reading/writing to sockets, and handling clients concurrently (here using threads per client). Consider edge cases: what if the client disconnects? (Your read will return 0 bytes). Use `Wireshark` to observe the traffic from your echo test – you’ll see the TCP handshake, the data you typed (as TCP payload), and the echo response.

* **3. Rust Async Echo (Optional):** Rewrite the echo server using **Tokio** for async handling instead of threads. Use `tokio::net::TcpListener` and `TcpStream`. Hint: use `listener.accept().await` inside an async loop, and spawn tasks with `tokio::spawn` for each accepted connection. Compare this with the threaded version: the functionality is the same, but Tokio can scale to many more connections by using an event loop (via mio under the hood) instead of one thread per client.

* **4. Explore TCP States:** On your machine, use the `ss` or `netstat` tool to inspect socket states. For example, run `ss -tanp` to list TCP sockets with their state and owning process. Start a long-lived connection (for instance, run `nc -l 9090` in one terminal and `nc localhost 9090` in another to simulate a persistent connection). Observe states: you should see `LISTEN` on port 9090 for the server, and an `ESTABLISHED` for the open connection. Terminate one side (Ctrl+C one of the nc processes) and run `ss -tanp` again – you might catch a socket in `FIN-WAIT-2` or `CLOSE-WAIT` state. Understanding these states will help when debugging issues like sockets stuck in TIME-WAIT or connections not closing properly.

* **5. Simulate Packet Loss (Advanced/Optional):** To really see TCP’s reliability, simulate a lossy network and observe retransmissions. On Linux, you can use `tc` (Traffic Control) to introduce packet loss on an interface. For example:

  ```bash
  sudo tc qdisc add dev lo root netem loss 20%  
  ```

  This will drop \~20% of packets on the loopback interface. Run your echo test or a simple `curl` under this condition. In Wireshark, observe that some TCP segments are not acknowledged and that the sender retransmits them (you’ll see duplicate sequence numbers marked “\[TCP Retransmission]”). You’ll also see out-of-order packets or duplicate ACKs from the receiver – which are clues to TCP that it needs to resend. (Don’t forget to remove the rule after: `tc qdisc del dev lo root netem`.)

* **6. UDP Fire-and-Forget:** As a quick contrast, use Rust’s `UdpSocket` to send a message. For example, try sending a UDP packet to an invalid host or closed port and see what happens (hint: `UdpSocket::send_to` will succeed even if no one is listening; UDP has no handshake). If you send to an unused port on your own machine, you can capture the ICMP “Port Unreachable” message sent back (since OS may respond with ICMP for unopened UDP ports). This highlights that UDP has no built-in acknowledgment.

After Day 2, you’ll have a deep understanding of TCP’s mechanics – you’ve *seen* the handshake and retransmission in action, written a basic TCP server, and gained familiarity with socket APIs. These are foundational for the next steps where we’ll build on TCP to implement higher-level protocols like HTTP.

---

## Day 3: Application Layer – HTTP/1.1 From the Ground Up

**Goals:** Dive into the Hypertext Transfer Protocol (HTTP) in its classic form (HTTP/1.1). Understand HTTP’s text-based message format, request/response lifecycle, and the stateless nature of the protocol. Learn about headers, methods, status codes, and how HTTP uses TCP under the hood (persistent connections and pipelining). Start building an HTTP server in Rust to solidify this knowledge, and use tools to inspect HTTP messages on the wire.

**Key Topics & Concepts:**

* **HTTP Basics:** HTTP/1.1 is a *stateless, application-level protocol* for distributed information systems. The client (usually a browser or tool like curl) sends a **request** message; the server replies with a **response** message. Key parts of an HTTP message:

  * **Start Line:** For requests: e.g. `GET /path HTTP/1.1`; for responses: e.g. `HTTP/1.1 200 OK`. This line indicates the request method, path, and version or the protocol version and status code.
  * **Headers:** Metadata in `Name: Value` format, one per line (e.g. `Host: example.com`, `Content-Type: text/html`). HTTP/1.1 headers are ASCII text and case-insensitive for names.
  * **Blank Line:** CRLF (`\r\n`) sequence separating headers from body.
  * **Body:** Optional, for POST/PUT and responses, can contain arbitrary data (HTML, JSON, etc.). The body length is signaled by headers like `Content-Length` or `Transfer-Encoding: chunked`.

* **Statelessness:** Each HTTP request is independent – the protocol itself doesn’t retain memory of previous requests. (State can be kept via cookies or other application-layer mechanisms, but the server treats each request on its own). This influenced web architecture heavily (e.g., requiring cookies for sessions).

* **Persistent Connections (Keep-Alive):** HTTP/1.0 opened a new TCP connection for each request by default. HTTP/1.1 introduced persistent connections – by default the connection stays open for reuse, sending multiple requests sequentially. The `Connection: keep-alive` header (default in 1.1) allows this, whereas `Connection: close` asks to close after one exchange. This saves the cost of TCP handshakes for every file (critical for pages with many assets).

* **Message Termination:** How does a server know when one HTTP request/response ends and another begins on a persistent connection? Either by `Content-Length` header (byte count of body), or by using **chunked transfer encoding** (for dynamic content of unknown length: body is sent in chunks prefixed by their size), or by closing the connection (for protocols like HTTP/1.0 or error cases). Understanding this is vital if implementing an HTTP parser.

* **HTTP Methods & Status Codes:** Know the common methods (GET, POST, PUT, DELETE, HEAD, OPTIONS) and what they mean. Know major status code classes: 2xx (success), 3xx (redirection), 4xx (client error), 5xx (server error). Specifically, 200 OK, 404 Not Found, 500 Internal Server Error, etc. are important to recognize. This is more on the semantic side but important for realistic server behavior.

* **Pipelining:** HTTP/1.1 allowed clients to send multiple requests without waiting for the first to finish (to better utilize the connection). In practice, this **pipelining** feature was rarely used and had issues (head-of-line blocking if an earlier response was slow, subsequent ones are stuck). Browsers largely did not enable it by default. This problem was one of the motivations for HTTP/2’s multiplexing.

* **Real-world HTTP 1.1 Performance Tricks:** To alleviate HTTP/1.1 limitations, browsers opened multiple parallel TCP connections (e.g., 6 per domain) to fetch resources concurrently. Also, techniques like **domain sharding** and **sprite images** were used to work around HTTP/1.1’s lack of multiplexing. (Keep these in mind; when we get to HTTP/2, these tricks become unnecessary.)

* **Rust HTTP Handling:** At a low level, handling HTTP means reading from a `TcpStream` and parsing text. We can either do this manually or use helper crates:

  * Low-level: Use `std::io::BufReader` to read lines and parse the request. The [`httparse`](https://docs.rs/httparse) crate can help parse HTTP 1.x headers if you don’t want to do it entirely from scratch.
  * High-level: The **Hyper** crate is the de-facto HTTP library in Rust (it powers many frameworks). Hyper can act as a client or server and supports HTTP/1.1 and HTTP/2. We will mostly do things “by hand” initially for learning, but it’s good to be aware that Hyper exists and even skim its docs to see how it represents requests and responses. Similarly, **Actix-web** is a high-level framework – we’ll touch on it later.

**Recommended Reading:**

* *HTTP/1.1 Spec:* RFC 9112 (HTTP/1.1, 2022) which covers HTTP/1.1 message syntax and connection management. Read sections 3 and 4 for message format and parsing rules (e.g., how header folding works, what characters are allowed). Also, RFC 9110 (HTTP Semantics) for definitions of methods, status codes, and headers (though you can refer to the simpler summaries on MDN for these).
* *MDN Web Docs:* “HTTP – Overview” on MDN for a friendly explanation of how HTTP requests and responses are structured. Also MDN’s page on [HTTP Headers](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers) to see common headers and their purpose.
* *High Performance Browser Networking:* The chapter “HTTP/1.x” gives insight into how HTTP/1.1 evolved, including persistent connections and pipelining (and why it still wasn’t enough). This will set the stage for appreciating HTTP/2 improvements.
* *HTTP Illustrated:* Consider browsing the *“HTTP Illustrated”* resource or other visuals that show an HTTP request timeline (with DNS lookup, TCP handshake, request, response). While not our main focus, understanding that full timeline (like what happens when you type a URL in a browser) is useful context.
* *Rust Crate Docs:* If using `httparse`, read its examples. If curious, skim Hyper’s [guide](https://hyper.rs/guide) to see how a high-level HTTP server is written (we’ll try something similar manually).

**Hands-On Exercises:**

* **1. Manually Speak HTTP:** Using a simple tool like **telnet** or **netcat**, manually send an HTTP request and observe the response. For example, in a terminal type:

  ```bash
  nc example.com 80  
  ```

  Once connected (you see a blank screen), type:

  ```
  GET / HTTP/1.1  
  Host: example.com  

  ```

  (Make sure to include the blank line after the headers.) You should see HTML come back. This exercise reinforces the plaintext nature of HTTP/1.1 – it’s just text over TCP. Try the same for a different site or a specific path. Observe status codes by requesting something like `GET /nonexistentpage HTTP/1.1`.

* **2. Build a Tiny HTTP Server (Rust):** Expand your Day 2 echo server to an HTTP server:

  * Accept a connection, read the request bytes. Parse at least the request line and headers up to the blank line. You can ignore the body for GET requests.
  * Formulate a basic response: e.g., for any request, return a fixed message with a minimal HTML. For instance, always respond with a 200 OK and a short HTML body. Example:

    ```
    HTTP/1.1 200 OK\r\n
    Content-Length: 13\r\n
    Content-Type: text/plain\r\n
    Connection: close\r\n
    \r\n
    Hello, world!
    ```

    Here we hard-coded `Content-Length: 13` bytes. This response will close the connection after sending (“Connection: close”).
  * Test this server using a browser or curl: `curl -v http://localhost:8080`. The `-v` (verbose) flag in curl will show the request it sends and the response received. Verify your server’s output matches HTTP format exactly (curl’s output will show the status line and headers). If something is off (missing CRLF, etc.), curl will inform you or just hang – use that feedback to fix your parser/response.
  * Once a basic response works, enhance it: e.g., log the request path and method to the console. Try to handle at least `GET /` vs `GET /favicon.ico` differently (maybe serve a static small favicon if you’re ambitious, or at least return 404 for unknown paths).
  * This exercise is critical – it’s essentially what a minimal web server does. You’ll appreciate things like persistent connections (for now, we closed after one response to keep it simple) and proper HTTP formatting. It also highlights why libraries like Hyper exist (parsing robustly is not trivial).

* **3. Persistent Connection & Multiple Requests:** Modify your server to support keep-alive (optional but illuminating). That means after sending a response, do **not** close the stream if the request had `Connection: keep-alive` (which HTTP/1.1 uses by default). You’ll need to loop back to read the next request on the same stream. You also must handle when the client eventually closes. Try testing with a browser (which might pipeline or at least reuse connections). This is tricky to get 100% right (especially parsing chunked bodies, etc.), so don’t worry if you simplify. The goal is to understand the concept: one connection, multiple sequential requests. Use Wireshark to confirm that the second request does not start with a new TCP handshake.

* **4. HTTP Client in Rust (Optional):** Write a simple Rust program to perform an HTTP GET over TCP (without using reqwest or hyper). Use `TcpStream` to connect to a server (e.g., `example.com:80`), then send a well-formed HTTP GET request as bytes, then read the response. This is basically what you did with netcat, but automated. It helps understand client behavior and ensure you know how to assemble requests properly. (Compare with the output of `curl -v` for correctness.)

* **5. Inspect HTTP with Wireshark:** Capture some HTTP traffic on your machine (perhaps from your own server or any HTTP (non-HTTPS) site since HTTPS will be encrypted). Apply filter `http`. Wireshark can actually parse HTTP details. Follow an HTTP stream to see the full request and response conversation. This is a great way to see headers and content and reinforces everything you’ve learned (it’s also enjoyable to “debug” web traffic this way).

By the end of Day 3, you’ve effectively built a toy web server, understand the structure of HTTP messages, and have used tools to generate and inspect HTTP requests. You now know *exactly* what happens on the wire when an HTTP request is made, which demystifies the web’s core protocol.

---

## Day 4: TLS and HTTPS – Security in Transport

**Goals:** Build a deep understanding of HTTPS by studying TLS (Transport Layer Security). Learn how TLS secures HTTP, the steps of the TLS handshake, and how certificates and encryption work in practice. Practice using tools to inspect TLS handshakes and set up a Rust server with TLS (using `rustls`). By the end of the day, you should know how HTTPS differs from HTTP and what it takes to implement secure connections.

**Key Topics & Concepts:**

* **TLS vs SSL:** TLS is the successor to SSL. Modern HTTPS uses TLS (SSL 2.0 and 3.0 are obsolete due to vulnerabilities). TLS 1.2 was standard for many years; TLS 1.3 is the latest (faster, more secure). Often colloquially still called “SSL”. Know that **HTTPS = HTTP over TLS** (HTTP messages encrypted inside a TLS session running on TCP port 443 by default).

* **TLS Handshake Process:** TLS handshake is an *Authenticated Key Exchange (AKE)* protocol that runs over a TCP connection. In a nutshell, the client and server:

  * exchange supported cryptographic algorithms (cipher suites),
  * perform key exchange (e.g., using Diffie-Hellman) to establish a shared secret,
  * authenticate the server’s identity (server presents an X.509 **certificate** to prove its identity),
  * derive symmetric session keys for encrypting the actual application data.

  In TLS 1.2, the handshake involves multiple round trips (ClientHello -> ServerHello -> Certificate -> \[ServerHelloDone] -> ClientKeyExchange -> etc.). In TLS 1.3, it’s simplified to 1-RTT: the server’s Certificate and key share come in its first response, and it can often complete handshake faster (and even 0-RTT for repeat connections). Key point: the handshake ensures both sides have a shared encryption key and the client has verified the server’s certificate, *before* any HTTP data is exchanged.

* **Certificates and PKI:** A TLS certificate (public key certificate) binds a public key to a server’s identity (domain name). It’s signed by a **Certificate Authority (CA)**. The client (browser) has a trust store of CA root certificates and uses that to verify the server’s cert chain. Key ideas:

  * The cert contains the server’s public key, domain name, issuer, validity period, etc.
  * The server proves possession of the corresponding private key (by signing something in the handshake).
  * Certificate chain: Server cert -> Intermediate CA cert -> ... -> Root CA. The handshake includes the chain (except root) and the client tries to build trust up to a root it knows.
  * Tools like `openssl s_client` or browser dev tools let you inspect the certificate sent by a server.

* **Symmetric Encryption & Keys:** TLS ultimately uses symmetric encryption (e.g., AES) with keys established in the handshake. Understand terms: **session key**, **cipher suite** (set of algorithms: key exchange method, cipher, MAC or AEAD). Also know about Perfect Forward Secrecy: cipher suites with Diffie-Hellman (DHE or ECDHE) provide PFS, meaning past traffic can’t be decrypted even if the server’s long-term key is compromised later.

* **Integrity and TLS Records:** TLS doesn’t just encrypt, it also ensures integrity (no tampering) via MAC or AEAD (Authenticated Encryption). Data is transmitted in TLS **records** that have a header, encrypted payload, and authentication tag. This is lower-level, but if you capture TLS traffic, you’ll see records (like “Application Data” records which carry encrypted HTTP bytes).

* **Browser Behavior:** When you hit `https://`, browser performs TCP handshake, then TLS handshake, verifies cert, then sends the HTTP request inside the secure channel. This adds latency (TLS 1.2 handshake typically \~2 round trips, TLS 1.3 \~1 RT). Modern improvements like session resumption and 0-RTT cut this down. There’s also an extension **ALPN (Application-Layer Protocol Negotiation)** in TLS which lets the server and client agree on the application protocol (e.g., HTTP/1.1 vs HTTP/2) within the TLS handshake – this is how a client indicates “I can do HTTP/2” when connecting, for example.

* **Rust TLS Libraries:** **rustls** is a pure-Rust TLS library (safe and modern). We’ll use `rustls` (or its async wrapper if using tokio, i.e., `tokio-rustls`) to add HTTPS to our server. There’s also OpenSSL bindings (`openssl` crate) but rustls is easier to use and avoids OpenSSL’s C dependency. We’ll need a certificate for testing – we can generate a self-signed cert for localhost.

**Recommended Reading:**

* *TLS 1.3 Spec:* RFC 8446 (TLS 1.3). Read the introductory sections to understand the handshake at a high level (Figure 1 in the RFC is useful to see message flow). You don’t need to digest the entire RFC (it’s very detailed), but get a sense of the flow of messages: ClientHello, ServerHello, etc.
* *Cloudflare’s TLS Handshake Explainer:* Cloudflare’s article “What happens in a TLS handshake?” is an excellent plain-language overview. It describes the steps and what’s accomplished (agree on version and cipher, exchange keys, verify certificate, etc.). This will reinforce the key points without getting lost in RFC jargon.
* *OpenSSL’s docs / Guide:* If new to TLS, the OpenSSL docs or Mozilla’s “Beginners guide to SSL/TLS” can help demystify terms like RSA, AES, MAC, etc., and how they come together in TLS.
* *rustls manual:* The rustls GitHub or docs often have examples of setting up a server. Review how to load certificates and keys, and how to wrap a `TcpStream` in a rustls `Stream` that handles encryption/decryption.

**Hands-On Exercises:**

* **1. Explore a TLS Handshake (openssl client):** Use OpenSSL’s client tool to connect to a website and dump handshake details:

  ```bash
  openssl s_client -connect example.com:443 -tls1_2 -msg  
  ```

  This connects with TLS 1.2 and `-msg` prints the handshake messages. You’ll see messages like `ClientHello`, `ServerHello`, `Certificate`, etc., with a lot of info (cipher suites, random bytes, etc.). It’s verbose, but scroll to see the **Certificate** message (it’ll show the server’s cert PEM). Try `-tls1_3` as well to see differences. Note: with `-msg`, the output shows raw handshake; without it, you’d get an interactive prompt after handshake where you could type an HTTP request (you can actually do `GET /` and it will respond). This tool is great for debugging TLS connections.

* **2. Inspect Certificate Details:** Still using `openssl s_client` (or you can use `openssl x509` with a file), inspect the certificate. After running the above, you’ll get a block of text “-----BEGIN CERTIFICATE----- ...”. Copy it (including BEGIN/END lines) to a file (say `cert.pem`). Run:

  ```bash
  openssl x509 -in cert.pem -noout -text  
  ```

  This prints the certificate fields (subject, issuer, valid dates, SANs, etc.). Confirm the **Common Name (CN)** or Subject Alternative Name matches the domain (example.com). See the issuer (which CA), and the validity period. This helps you understand the PKI aspect of TLS.

* **3. Generate a Self-Signed Cert for localhost:** We need a cert to run our own HTTPS server. Easiest way:

  ```bash
  openssl req -x509 -newkey rsa:2048 -nodes -keyout key.pem -out cert.pem -days 3 -subj "/CN=localhost"  
  ```

  This generates a new RSA key and a self-signed certificate valid for 3 days for “localhost”. (Self-signed means it’s not signed by a known CA, but for our testing that’s fine; the browser will warn if you use it, but curl can be told to ignore verification with `-k`). Now you have `cert.pem` and `key.pem`.

* **4. Add TLS to Rust Server:** Using **rustls**, wrap your Day 3 HTTP server so it can speak HTTPS. Steps:

  * Include the `rustls` crate (and `tokio-rustls` if using Tokio).
  * Load the `cert.pem` and `key.pem` you generated into a rustls `Certificate` and `PrivateKey`. (rustls expects DER-encoded; you can use the `rustls::internal::pemfile` module to parse the PEM).
  * Create a rustls `ServerConfig` and configure it with the cert and key. Choose a cipher suite set (rustls has defaults).
  * Instead of `TcpListener::bind("0.0.0.0:8080")`, use that to accept but then for each incoming `TcpStream`, perform a `rustls::ServerSession` (or if using `tokio_rustls`, an `TlsAcceptor`) to handle the TLS handshake.
  * Once the TLS handshake is done, you get an encrypted stream (implementing `Read/Write` traits) – you can largely reuse your HTTP handling code on that stream (the data you read will be decrypted application data).

  This is a complex task if you haven’t used rustls before, but it’s extremely educational. If stuck, refer to rustls examples or consider using an easier approach: the `openssl` crate to wrap the stream with an `SslStream` (slightly simpler API if you are familiar with OpenSSL). Another approach is to use Actix or Hyper just to see how they simplify HTTPS (actix-web can do HTTPS by a few config lines). But try with rustls for learning.

  Test your HTTPS server: `curl -v https://localhost:8443 -k` (if you bind to 8443, common for local). The `-k` flag tells curl to skip cert verification (since it’s self-signed). You should see the HTTPS handshake in curl’s verbose output and then your response. Also try from a browser: it will likely give a warning (“self-signed certificate” or “not secure”). You can bypass it for testing. If successful, you have a basic HTTPS server!

* **5. Analyze HTTPS Traffic in Wireshark:** Capture the traffic from your curl/browser to your local HTTPS server. By default, you’ll just see “ClientHello... Application Data” and not the plaintext. However, because you have the server’s key, we can decrypt the TLS session in Wireshark:

  * Go to Preferences > Protocols > TLS, and add your key (`key.pem`) under RSA keys list (IP,port,protocol, key file, and cert). Wireshark (for TLS1.2 and below, or TLS1.3 if key log or RSA) can then decrypt. If you used RSA and no Diffie-Hellman (in your openssl req command we did RSA; rustls by default may use ECDHE which complicates decryption without key logs). For simplicity, you could force a cipher suite with RSA key exchange in rustls for this test (or use an older TLS1.2 with RSA kx, which is not PFS).
  * Alternatively, run your client (e.g. curl) with an environment variable `SSLKEYLOGFILE=./keys.log` (supported by some TLS libraries like NSS, used by Firefox/Chrome or curl if built with it). This log allows Wireshark to decrypt TLS 1.3 too by providing session keys.
  * If set up, Wireshark will show HTTP inside the TLS stream (you’ll see the GET and response just like Day 3). This step is optional and a bit fiddly, but it solidifies understanding that *HTTPS is HTTP inside TLS*. When TLS is properly implemented, eavesdroppers (or Wireshark without keys) can’t see the HTTP content – just the handshake and encrypted blobs.

* **6. Experiment with TLS Versions/Ciphers:** Using `openssl s_client` or configuring your rustls, try disallowing certain ciphers or versions. For example, in rustls you can enable TLS1.3 only, or TLS1.2 only. Use `openssl s_client -connect localhost:8443 -tls1_2` to ensure your server supports TLS1.2. You can also inspect cipher suites chosen. This will deepen your understanding of compatibility issues and the negotiation aspect of TLS.

After Day 4, you should appreciate what it takes to secure communications. You’ve set up a server with HTTPS, seen how certificates work, and how the TLS handshake establishes a secure channel. Crucially, you now know that **HTTPS = HTTP + encryption + authentication**. This knowledge prepares you for understanding modern protocols built on TLS (like HTTP/2 and WebSockets over TLS).

---

## Day 5: HTTP/2 – Binary, Multiplexed HTTP

**Goals:** Understand the improvements HTTP/2 brings over HTTP/1.1: binary framing, multiplexing, header compression, and request prioritization. Learn how HTTP/2 rides on top of TLS (typically) and how it affects web performance. Experiment with HTTP/2 using tools (like `curl --http2` or browsers) and analyze HTTP/2 frames with Wireshark or logs. Optionally, use Rust’s HTTP/2 support (via Hyper or the `h2` crate) to make requests or implement a basic server behavior.

**Key Topics & Concepts:**

* **Why HTTP/2?** Recognize the problems of HTTP/1.1: no concurrency on one connection (leading to head-of-line blocking and the need for multiple connections), and verbose repetitive headers (which waste bandwidth, especially over high-latency links). HTTP/2 was developed (originally from Google’s SPDY) to address these issues.

* **Binary Framing Layer:** HTTP/2 is not textual like 1.1; it introduces a binary protocol where HTTP messages are split into frames. Key frame types include **HEADERS**, **DATA**, **SETTINGS**, **WINDOW\_UPDATE**, **PUSH\_PROMISE**, etc.. The basic idea: a single TCP connection can carry many streams of messages interleaved, with each frame belonging to a stream identified by an ID.

* **Streams & Multiplexing:** A **stream** in HTTP/2 is a bidirectional logical channel within the TCP connection. Each request/response pair corresponds to a stream (client-initiated streams have odd IDs typically). Multiplexing means a client can send multiple requests at once on the same connection without waiting, and the responses can come back out of order (interleaved), since frames of different streams are mixed. This solves the head-of-line blocking problem at the application layer – one slow response doesn’t prevent others from using the connection. Note: TCP is still underneath, so if a TCP packet is lost, it affects all streams (TCP’s own HOL blocking), but that’s a separate matter (addressed by HTTP/3 with QUIC).

* **Header Compression (HPACK):** HTTP/2 compresses headers using HPACK (Huffman coding + static/dynamic header tables). Instead of resending the same header strings for every request (think of 10 requests all sending user-agent, cookies, etc.), HTTP/2 sends them once and then refers to them by index. This greatly reduces overhead for pages with many requests. Understand at a high level that HPACK maintains a table of headers on both sides.

* **Server Push:** HTTP/2 allows the server to push resources the client has not requested yet, to avoid extra round-trip. E.g., when client requests page, server can *push* CSS/JS resources it knows the page needs, without waiting for client to ask. The client can reject pushes if not needed. This is a nice feature but has to be used carefully (browsers have had mixed support/tuning for it).

* **Prioritization:** HTTP/2 has optional stream prioritization. The client can indicate which streams are more important (or dependent on others). In practice, browsers do send priority info (so that e.g. visible images load before hidden ones, etc.), but many servers don’t heavily use this info or have simplified handling. Still, know the concept: frames like PRIORITY can set weights/dependencies.

* **TLS and ALPN:** HTTP/2 is most often used over TLS (h2 over TLS, whereas h2c is the cleartext version that’s rarely used except in some internal networks or prior upgrade from HTTP/1.1). To initiate HTTP/2, clients use **ALPN** during TLS handshake to negotiate h2. If the server agrees, the connection speaks HTTP/2. If not, it might speak HTTP/1.1. (Browsers only use HTTP/2 with TLS except for localhost or prior known exceptions, due to concerns about middleboxes interfering with a new protocol on port 80). For our purposes, we’ll assume HTTP/2 over TLS.

* **Rust HTTP/2 Support:** The `hyper` crate supports HTTP/2 for both client and server (when used with TLS and ALPN via rustls or native-tls). There’s also the lower-level [`h2`](https://crates.io/crates/h2) crate that implements the HTTP/2 protocol (you could use it to handle frames if you wanted to build a custom server). We might not implement an HTTP/2 server from scratch (that’s a lot!), but we can use hyper as a client to fetch something with HTTP/2 or stand up a hyper server that speaks h2 automatically.

**Recommended Reading:**

* *HTTP/2 Spec:* RFC 7540 (original HTTP/2) or the updated RFC 9113. Read the introduction for motivation and Section 5 to understand the concept of streams, and Section 6 for frame types. You don’t need to memorize frame layouts, just know the roles of HEADERS vs DATA frames and how messages are divided.
* *HTTP/2 Explained (by Daniel Stenberg)*: There’s a free ebook “HTTP/2 Explained” by Daniel Stenberg. Skim through chapters that explain the concepts in a more digestible way than the RFC. Focus on chapters covering multiplexing and header compression.
* *High Performance Browser Networking:* The chapter on HTTP/2 in HPBN is excellent to understand *practical* benefits. It discusses how HTTP/2 impacts performance and what it means for best practices (e.g., it explicitly says domain sharding and image spriting are no longer needed).
* *Wireshark HTTP/2 Wiki:* Wireshark has documentation on how to decrypt and dissect HTTP/2. It can show streams and frame details if TLS keys are known.

**Hands-On Exercises:**

* **1. Enable HTTP/2 in Curl:** Pick a URL that you know supports HTTP/2 (almost all modern HTTPS sites do). Run:

  ```bash
  curl -v --http2 https://www.google.com/ -o /dev/null  
  ```

  The `-v` output will show something like `ALPN, server accepted to use h2` and then it might show HTTP/2 specifics, or at least it will confirm it used HTTP/2. Curl’s verbose output for HTTP/2 is not extremely detailed, but it will show headers. Alternatively, use `nghttp` (from nghttp2 project) which is a command-line HTTP/2 client that prints frame info. If you have `nghttp`, try: `nghttp -ans https://example.com` (the `-ans` flags: show ASCII, no dependencies, and TLS info). This will dump frames and content.

* **2. Observe Multiplexing:** A fun test – find a website that takes a bit of time to respond or artificially create one (you could simulate with two endpoints on a test server: one that waits a few seconds, one quick). Use a client that can do parallel requests. For instance:

  ```bash
  curl -v --http2 -o /dev/null https://http2.akamai.com/demo/tile-1.png https://http2.akamai.com/demo/tile-2.png  
  ```

  (Akamai’s demo site might allow concurrent fetches of image tiles.) In HTTP/1.1, these would likely be serialized or require two connections; in HTTP/2, they’ll share one connection. Use `time` or look at curl’s timings to see they were fetched concurrently. Alternatively, open Chrome DevTools (Network tab) and load a site over HTTP/2 – you can see in the waterfall that requests start together rather than one-by-one.

* **3. Wireshark HTTP/2 Frame Analysis:** If you can capture HTTP/2 traffic and decrypt it (similar to Day 4’s step, using SSLKEYLOGFILE), Wireshark will show HTTP/2 protocol details. It will list streams and frames. Try to identify a HEADERS frame (opening a new stream with request headers) and DATA frames (carrying the response body). This cements how the “text” of HTTP is now binary. You’ll see something like `:method: GET` and other headers in a HEADERS frame decode. If decrypting real traffic is too hard, you can also use `nghttp` in a verbose mode to see frames.

* **4. Simple Rust HTTP/2 Client:** Use the hyper crate’s client to make an HTTP/2 request. Hyper by default will negotiate HTTP/2 if you use the TLS feature and the server supports it. For example, use `hyper::Client<hyper_tls::HttpsConnector<hyper::client::HttpConnector>>` to create an HTTPS client (or use `reqwest` which is built on hyper). Make a GET request to a known HTTP/2 site. You can verify in the response or via logs that HTTP/2 was used (hyper might provide a way to check the protocol used; if not, you can turn on some log or use wireshark to confirm). The goal is mostly to see that from a developer perspective, you don’t have to handle the frames – the library does it. If you print response headers, you might see the HTTP/2 pseudo-headers (like `:status`) in some context.

* **5. HTTP/2 Server Push (Optional):** If you’re curious, try a demo of server push. The Akamai HTTP/2 demo site or other sites may demonstrate it. For instance, [https://http2.akamai.com/](https://http2.akamai.com/) has a demo where the server pushes images. Use DevTools to see the push in action (pushed resources might be indicated). Pushed responses usually appear with an indicator in devtools or a header like “x-http2-pushed”. This is just to be aware of the feature, though push is not widely utilized in the wild or is being reconsidered in HTTP/3.

* **6. Experiment with a Rust HTTP/2 Server (Advanced):** If you have time, try to spin up a hyper-based server in Rust that serves HTTP/2. Hyper will manage the protocol if configured with HTTP2 enabled (requires TLS ALPN negotiation). Essentially, you’d integrate your Day 4 rustls config with hyper’s server builder. This is non-trivial, but there are examples (search “hyper rustls HTTP2 server example”). The simplest way might be to use **Actix-web**: by default, Actix-web will use HTTP/1.x, but if you run it with TLS, it can speak HTTP/2. You could adapt one of Actix’s examples to serve a few routes and test with curl to confirm HTTP/2. This exercise shows how frameworks abstract away the complexity – you focus on routes and handlers, and the framework deals with the binary protocol details.

At the end of Day 5, you should grasp how HTTP/2 keeps the same semantics as HTTP/1.1 (still requests and responses) but packages them very differently for efficiency. You’ve seen or experimented with the multiplexing that solves many HTTP/1.1 limitations. This prepares you to handle the final piece – WebSockets – which will introduce full-duplex communication in our stack.

---

## Day 6: WebSockets – Real-Time Full-Duplex Communication

**Goals:** Learn about the WebSocket protocol, which enables persistent two-way communication between client and server over a single connection. Understand how WebSockets upgrade from an HTTP handshake and then operate over TCP. Implement a simple WebSocket server (e.g., an echo or chat server) in Rust (using a library like `tungstenite` or `tokio-tungstenite`). Practice using browser DevTools or a WebSocket client to send messages and observe frames.

**Key Topics & Concepts:**

* **WebSocket Purpose:** Unlike HTTP request/response (which is client-initiated and half-duplex), WebSockets provide a *full-duplex*, message-oriented channel over a single TCP connection. This is great for chat apps, live updates, gaming, etc., where server can push data to client asynchronously and client can send data anytime after connection.

* **Handshake via HTTP/1.1 Upgrade:** The WebSocket protocol begins with an HTTP/1.1 request that requests an upgrade to WebSocket. The client sends a request like:

  ```
  GET /chat HTTP/1.1  
  Host: server.example.com  
  Upgrade: websocket  
  Connection: Upgrade  
  Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==  
  Sec-WebSocket-Version: 13  
  ```

  The server, if it accepts, replies:

  ```
  HTTP/1.1 101 Switching Protocols  
  Upgrade: websocket  
  Connection: Upgrade  
  Sec-WebSocket-Accept: <computed key>  
  ```

  This “101 Switching Protocols” response indicates the HTTP connection is now switching to the WebSocket protocol. After this, HTTP is no longer in play; it’s raw WebSocket frames over the same TCP.

* **Sec-WebSocket-Key/Sec-WebSocket-Accept:** The client sends a random base64 key; the server must respond with a hash value (key + a GUID, SHA-1, base64). This is a security measure to ensure the client is talking to a real WebSocket server (and not some non-WebSocket endpoint that accidentally responds). As a developer, you often use library functions to compute this accept key. The algorithm: take the `Sec-WebSocket-Key` the client sent, append the magic string `258EAFA5-E914-47DA-95CA-C5AB0DC85B11`, compute SHA-1 hash, then base64 encode – that’s the `Sec-WebSocket-Accept` value.

* **WebSocket Protocol (RFC 6455):** Once upgraded, WebSocket defines its own frame structure:

  * A small header with bits: FIN (is this the final fragment of a message), opcode (e.g. 1 = text frame, 2 = binary frame, 8 = close, 9 = ping, 10 = pong), a “mask” flag, and payload length.
  * Client-to-server frames **must be masked** with a random 32-bit mask (for security reasons to prevent certain proxies from misinterpreting data). The server will unmask it. Server-to-client frames are not masked.
  * You can have continuation frames if a message is sent in parts. But most usage sends one message in one frame (with FIN=1).
  * Control frames like ping/pong and close exist for keepalive and closing handshake.
  * Importantly, WebSockets is message-oriented (a message can be split across frames but typically one frame = one message for simplicity). And it’s full-duplex: client and server can send frames independently at any time.

* **Using WebSockets:** In the browser, you’d use the WebSocket JavaScript API (`new WebSocket("wss://server/endpoint")`) to open a connection. `ws://` is for insecure, `wss://` for secure (WebSocket over TLS, analogous to HTTPS). Once open, `socket.send(...)` and an onmessage handler to receive events.

* **WebSocket vs HTTP/2 vs SSE:** It’s useful to note: HTTP/2 and upcoming HTTP/3 allow server push and bi-directional streams to some extent, but WebSocket remains a distinct, widely-used solution for truly full-duplex communication especially initiated from browsers. Server-Sent Events (SSE) is another mechanism (simpler, one-way server-to-client over HTTP). Each has its use cases, but WebSocket is the go-to for interactive comms.

* **Rust WebSocket Libraries:** **tungstenite** (synchronous) and **tokio-tungstenite** (async) are popular. High-level frameworks like Actix and Warp also have WebSocket support (they manage the upgrade and then provide an API to send/receive messages in handlers). We’ll likely use tokio-tungstenite in an example to make it easier.

**Recommended Reading:**

* *RFC 6455 (WebSocket):* Read the opening sections for an overview. Section 1 and 4 describe the handshake and security considerations. You can also read the summary on MDN or other tutorials for the WebSocket handshake (the key exchange and upgrade flow).
* *“WebSocket explained” (http.dev):* The resource we saw gives a concise explanation of the upgrade request and response with examples. It’s a good recap of what headers are involved.
* *MDN WebSockets API:* Understand how a WebSocket is used from JavaScript perspective (this helps ensure your server interacts correctly). MDN’s WebSocket API docs show example client code which you can use to test your server.
* *Tokio Tungstenite docs:* Browse the README or docs of `tokio-tungstenite` on how to accept a connection. Also, the `warp` or `actix-web` docs if you want to see how frameworks handle WebSockets (they usually give you a callback or stream for messages).

**Hands-On Exercises:**

* **1. WebSocket with Browser Developer Tools:** Find a site that uses WebSockets (for example, an online chat or perhaps a simple echo test service). Many online demos exist (e.g., **echo.websocket.org** used to exist as a public echo server, but if not, you can quickly set up your own as below). In Chrome DevTools (Network tab), you can filter by “WS” to see WebSocket connections. Click one and you can see messages being sent and received in real time. This is a great way to visualize the message flow. (If you can’t find a live example easily, skip ahead and do exercise 3, then use your own server with devtools.)

* **2. Simple WebSocket Client (Rust or Python):** Before building the server, it might be insightful to create a short client script that connects to a known WebSocket server. You could use Rust with `tungstenite::connect` to `ws://echo.websocket.events` (there are public echo servers for testing) and send a message, then print the response. Even a Python snippet with `websockets` library could do. This just verifies understanding of the client side and that the echo service bounces your message.

* **3. Build a WebSocket Echo Server:** Time to extend your server again. We’ll do this using a library to avoid dealing with frame bytes manually:

  * If sticking with our Day 3/4 server base (which is synchronous), use `tungstenite` crate’s `accept` function on a `TcpStream` that has received an HTTP Upgrade request. Tungstenite can handle the handshake for you. Once upgraded, it gives you a `WebSocket` object where you can `read_message` and `write_message`.
  * If using Tokio, `tokio_tungstenite::accept_async` does the same for an async stream.
  * Essentially: when you get a request with `Upgrade: websocket`, validate it (at least check the `Sec-WebSocket-Key` and `Version`), then call the accept function to do the handshake (it will handle computing `Sec-WebSocket-Accept` and sending the 101 Switching Protocols response).
  * After that, you enter a loop of reading messages and echoing them back. Each message from the client can be printed or logged, and you send it back. Handle the close message when encountered (or break out if `read_message` says the connection closed).
  * Use threads or tasks for each connection as needed (similar to how we handled TCP clients).

  Test it: Open a browser console and do:

  ```js
  let ws = new WebSocket("ws://localhost:8080/chat");
  ws.onmessage = e => console.log("Received:", e.data);
  ws.onopen = () => { ws.send("Hello!"); };
  ```

  You should see the message echoed back in the console. Alternatively, use `wscat` (a websocket CLI tool) or `websocat` to connect and interact via terminal.

* **4. Handling Multiple Clients (Broadcast):** Enhance the server (optional) to keep track of multiple connections and broadcast messages to all (a simple chatroom). This requires a shared list of client sockets (you’ll need synchronization – e.g., a `Arc<Mutex<Vec<...>>>` of connections, or if using async, maybe an `broadcast` channel). When one client sends a message, loop through and send to everyone (including original sender or not, your choice). This is an exercise in basic server state management and concurrency. Test by opening two browser windows (or ws clients) and ensure messages from one show up in the other.

* **5. Inspect WebSocket Frames:** Use Wireshark to capture the WebSocket traffic (if unencrypted, just `ws://`). Wireshark will identify it as “WebSocket” protocol and you can see the payload of each frame. You’ll notice the **masking key** on frames from client (Wireshark will automatically unmask and show payload). This reinforces how the WebSocket layer is just a thin framing on top of TCP. If you use wss (WebSocket Secure), then you’d need to decrypt TLS as before to see inside.

* **6. Error Handling and Edge Cases:** Try sending binary data versus text, see that tungstenite (and browser) can handle both (maybe send an ArrayBuffer from the browser). Also experiment with sending a large message to see fragmentation (tungstenite will handle it, but you can configure a max message size). Understanding these ensures you appreciate the differences between streaming (TCP) and messaging (WebSocket message might be split but you usually get it whole via the library).

By the end of Day 6, you have essentially built a real-time server that goes beyond request/response. You understand how a WebSocket begins as an HTTP handshake and then becomes its own protocol. You’ve likely used both browser tools and Rust code to handle WebSocket messages. This completes our coverage of the major protocols in the stack.

---

## Day 7: Integration, Projects & Advanced Topics

**Goals:** Consolidate everything learned by building a complete mini-application that ties together TCP, HTTP/HTTPS, and WebSockets. Explore any remaining advanced topics such as performance tuning, HTTP/3 (the future of HTTP), and further reading to continue your journey. By the end of this day, you should have a holistic understanding of network layers and be comfortable debugging at any layer from OSI 1 through 7!

**Project – Build a Mini Web Server with WebSockets (in Rust):**
For the final day, the capstone project will be to create an **“AwesomeServer”** that supports the following: serve a basic web page over **HTTPS**, and implement a WebSocket endpoint for real-time communication. You will utilize many components from previous days:

* Use **Tokio** as the asynchronous foundation to handle multiple connections concurrently and efficiently.
* Use **Rustls** for TLS to support HTTPS (port 443).
* Use either **Hyper** or low-level parsing to handle HTTP requests for the web page.
* Use **Tokio-Tungstenite** to handle the WebSocket upgrade and messaging.
* Optionally, use **Actix-web** or **Warp** framework to simplify some parts or as a comparison, but try to implement core logic yourself for learning.

**Step-by-Step Plan for Project:**

1. **Setup:** Decide if you’ll build on the previous code or start fresh. Given the complexity, you might choose a framework like Actix-web to avoid reimplementing HTTP parsing again, focusing on integration. However, since this is a learning exercise, doing some manual handling is okay. Ensure you have your `cert.pem` and `key.pem` (from Day 4) for TLS.

2. **Serve a Static HTML:** Create a simple `index.html` that will act as the client page. It should include a basic UI for a chat – e.g., an input box and a message log, plus a small script to connect via WebSocket to your server and append messages. For example:

   ```html
   <!-- index.html -->
   <h1>WebSocket Chat</h1>
   <ul id="messages"></ul>
   <input id="chatInput" type="text" placeholder="Type a message..."/>
   <script>
     const ws = new WebSocket("wss://yourdomain_or_localhost/chat");
     ws.onmessage = (event) => {
       const li = document.createElement('li');
       li.textContent = 'Friend: ' + event.data;
       document.getElementById('messages').append(li);
     };
     document.getElementById('chatInput').addEventListener('keydown', e => {
       if(e.key === 'Enter') {
         ws.send(e.target.value);
         const li = document.createElement('li');
         li.textContent = 'Me: ' + e.target.value;
         document.getElementById('messages').append(li);
         e.target.value = '';
       }
     });
   </script>
   ```

   (This simple script will connect to `wss://.../chat` and send messages on Enter, showing sent and received messages.)

3. **Server HTTP Handling:** Your server should serve this `index.html` when a client requests `/` over HTTP GET. If using Hyper or Actix, define a route for `/` that returns the HTML file with `Content-Type: text/html`. If doing manually, when you parse an HTTP GET for `/`, respond with status 200 and the file content (you can embed the HTML in a `const` or read from disk). Ensure you handle TLS – probably easiest is to terminate TLS at the socket accept stage (using rustls as before), then you get a plaintext HTTP request to handle.

4. **WebSocket Endpoint:** The client will connect to `wss://yourserver/chat`. So your server needs to handle `GET /chat` with `Upgrade: websocket`. If using a framework:

   * In Actix-web, for example, you’d have an endpoint `/chat` that upgrades using `actix_web::web::Payload` and their WS support (they give you a `WebsocketContext`).
   * In a manual/Hyper approach: detect the `Upgrade` header in the request. Use tungstenite’s accept to do the handshake. From that point, you’ll spawn a task to handle the WebSocket messages (since now it’s an independent continual conversation).
   * Manage a list of connected clients as in Day 6 if you want broadcast functionality.

5. **Broadcast (or Echo) logic:** For a chat, you’d broadcast messages. So you need some shared state (e.g., `Arc<Mutex<Vec<Sender>>>` where each Sender is an mpsc channel to a connected client task, or directly store their `WebSocket` objects if synchronous). When a message is received from one client, iterate over others and send it out. (Be careful to handle disconnects – remove broken connections from the list.)

6. **Test the full system:** Launch your server (with TLS on appropriate port, 443 if possible or 8443 if easier as non-root). Open two browsers to `https://...` and ensure:

   * The page loads (served via HTTPS).
   * The WebSocket connects (you might see a certificate warning if self-signed; proceed after accepting).
   * Sending messages in one browser causes them to appear in the other and vice versa.
     Use browser devtools to monitor the network: you should see the initial HTML fetched via HTTPS (HTTP/1.1 or 2 depending on your server config), and a WebSocket connection upgraded (101 Switching Protocols), then messages flowing. This is a full integration of everything!

7. **Deploy or Simulate Real Conditions (optional):** If you can, test the server over the internet or simulate latency. For instance, run the server on a cloud VM or container and access it via its domain. Or use `tc` to add delay/packet loss to see how it performs. WebSockets will keep going even with some hiccups (TCP handles reliability).

This project touches on all layers: you have low-level networking (TCP), security (TLS), HTTP parsing, and application protocol design (WebSocket messaging). Even though we leaned on libraries, you should now conceptually understand each piece’s role.

**Advanced Topics & Further Exploration:**

* **HTTP/3 and QUIC:** The next evolution of HTTP is HTTP/3, which uses QUIC (built on UDP) to avoid TCP head-of-line blocking and improve latency (especially in high-loss networks). HTTP/3 is already being deployed (RFC 9114 for HTTP/3). It’s worth reading up on QUIC’s approach: it implements reliability on UDP and multipath, etc. As an experienced backend dev, you will encounter HTTP/3 soon. A quick read: Cloudflare’s blog on HTTP/3 and QUIC, or the official “HTTP/3 explained” by Stenberg. Tools like `curl --http3` can already test it (if curl built with HTTP/3 support and target server supports it).
* **Performance Tuning:** Dive deeper into topics like TCP congestion control variants (CUBIC vs BBR), TLS session resumption (tickets, session IDs), and HTTP caching semantics (RFC 9111). These impact real-world performance and scalability.
* **Security:** Explore HTTPS client certificate authentication, mutual TLS, and also things like HSTS (HTTP Strict Transport Security header) which tells browsers to enforce HTTPS. Understand common vulnerabilities at each layer (e.g., TCP RST attacks, TLS downgrade attacks, WebSocket CSRF or injection if not careful, etc.).
* **Monitoring and Debugging Tools:** Learn tools like **Wireshark** in depth (display filters, follow stream, exporting objects from HTTP, etc.), **tcpdump** with complex filters (proto/port/byte matching), and other tools like **traceroute** (which we touched on), **mtr** (continuous traceroute), **ss** (socket stats), **iftop** or **nload** (bandwidth monitoring). For application-layer debugging: **cURL** for HTTP, **OpenSSL** for TLS, **WebSocket CLI** tools for WS. Being proficient with these makes you a networking troubleshooter.
* **RFCs & Specs to Read:** We used many RFC references – consider reading some in full as time permits:

  * RFC 1122 (Requirements for Internet Hosts) – a foundational doc that outlines how TCP/IP hosts should behave.
  * RFC 7230-7235 (the older HTTP/1.1 spec set) or the newer RFC 9110+ for HTTP semantics, caching, auth.
  * RFC 6455 (WebSocket) for a deeper understanding of frame format and the reasoning behind mask, etc.
  * RFC 8446 (TLS1.3) if you enjoy security, as it’s much cleaner than TLS1.2 and the RFC is quite readable in its design rationale sections.
* **Network Layer Deep Dives:** If interested, explore routing protocols (OSPF, BGP), which are beyond our scope but fascinating as “under the hood of the Internet”. Also, learn about NAT and how TCP/UDP flows are translated – important for backend developers to understand connectivity issues in cloud environments (e.g., why WebSockets might need pings to keep NAT mappings alive).

**Tools Reference:** (cheat-sheet table of tools and their use)

| Tool                  | Purpose                                                                                                | Example Usage & Tips                                                                                                                                                                                                                                                         |
| --------------------- | ------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Wireshark**         | GUI Packet Analyzer – deep inspection of packets at any layer. Supports filters and protocol decoding. | Use display filters like `tcp.port == 443` or `http && ip.addr == 1.2.3.4`. Right-click a TCP packet and “Follow TCP Stream” to see reassembled data (useful for HTTP). Use Wireshark to debug handshake issues (e.g., see if SYNs are sent, TLS ClientHello details, etc.). |
| **tcpdump**           | CLI Packet Capture – lightweight and scriptable.                                                       | `tcpdump -n -i eth0 port 80 -w http.pcap` to capture HTTP traffic on eth0 to a file. Use `-X` or `-A` to view content in ascii/hex for text protocols. Great for quick remote debugging (can combine with ssh).                                                              |
| **curl**              | HTTP/HTTPS Client CLI – for testing HTTP requests, including HTTP/2 and HTTP/3.                        | `curl -v https://example.com` (verbose output with headers). `curl --http2 -k https://localhost:8443/chat -d "hello"` to test HTTP/2 POST, etc. Use `-H` to add headers, `-X` to change method. Indispensable for REST API testing.                                          |
| **openssl s\_client** | TLS Client & Debug – initiate TLS connections and inspect handshake/certs.                             | `openssl s_client -connect host:443` basic connection (will show server cert). Use `-msg` or `-state` to debug the handshake messages. Use it to test particular TLS versions or cipher suites (with `-tls1_2`, `-cipher`).                                                  |
| **netcat (nc)**       | Swiss-army knife for TCP/UDP – can act as client or server or send raw data.                           | `nc -l 12345` to listen on port 12345 (TCP). `nc host 80` to open a TCP connection to host. You can send raw HTTP requests by typing. Also can transfer files or test ports.                                                                                                 |
| **ss / netstat**      | Socket statistics – see open connections and listening ports, and their states.                        | `ss -tuna` (all TCP/UDP, numeric addresses). Check for connections in TIME-WAIT or if a port is listening. Useful to ensure your server is bound to the right interface/port and to diagnose port exhaustion or leftover sockets.                                            |
| **traceroute / mtr**  | Path tracing – see the route packets take and measure per-hop latency.                                 | `traceroute 8.8.8.8` (on Windows use `tracert`). `mtr google.com` gives a continuously updated traceroute. This helps in understanding network issues (like if there’s a delay outside of your server).                                                                      |
| **wscat/websocat**    | WebSocket CLI clients – test WebSocket servers easily from terminal.                                   | `npx wscat -c ws://localhost:8080/chat` (using Node’s wscat via npx) to connect to your WS and allow typing messages. Websocat is a Rust alternative (`websocat ws://...`). Great for quick manual testing of WS without a browser.                                          |

Throughout this week, you’ve built up a mental model and practical skills from the physical layer (well, we conceptually covered it) all the way to application protocols. You’ve read and referenced primary specifications (RFCs) for authoritative details, and you’ve applied the knowledge in hands-on projects. By compressing what is often a multi-week course into 7 intensive days, you’ve achieved a comprehensive understanding of **TCP/IP, HTTP, HTTPS, HTTP/2, and WebSockets** – truly mastering what happens under the hood when applications communicate over networks.

Use this foundation to further explore and build even more complex and high-performance network applications – whether it’s tuning kernel parameters for thousands of connections or debugging the next generation of protocols, you now have the tools and knowledge to proceed with confidence. Happy networking!

