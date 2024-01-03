/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"

/* This is a simple WebSocket echo server example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */

/* ws->getUserData returns one of these */
struct PerSocketData {
    /* Fill with user data */

};

int main() {

    int port = 9001;

    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    auto app = uWS::App({
        /* There are example certificates in uWebSockets.js repo */
	    .key_file_name = "misc/key.pem",
	    .cert_file_name = "misc/cert.pem",
	    .passphrase = "1234"
	}).ws<PerSocketData>("/*", {
        /* Settings */
        // .compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR_4KB | uWS::DEDICATED_DECOMPRESSOR),
        // .maxPayloadLength = 100 * 1024 * 1024,
        // .idleTimeout = 16,
        // .maxBackpressure = 100 * 1024 * 1024,
        // .closeOnBackpressureLimit = false,
        // .resetIdleTimeoutOnSend = false,
        // .sendPingsAutomatically = true,
        /* Handlers */

        /* Client Initiating Socket Connection */
        .upgrade = [](auto *res, auto *req, auto *context) {
            
            /* You may read from req only here, and COPY whatever you need into your PerSocketData.
             * PerSocketData is valid from .open to .close event, accessed with ws->getUserData().
             * HttpRequest (req) is ONLY valid in this very callback, so any data you will need later
             * has to be COPIED into PerSocketData here. */

            res->template upgrade<PerSocketData>(
                {},
                req->getHeader("sec-websocket-key"),
                req->getHeader("sec-websocket-protocol"),
                req->getHeader("sec-websocket-extensions"),
                context);
        },

        /* Socket just opened */
        .open = [](auto *ws) {
            PerSocketData *socketData = ws->getUserData();
 
        },

        /* Client communicating a message to server */
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            std::string messageString = static_cast<std::string>(message);

        },

        /* Cleanup resources with socket closes */
        .close = [](auto *ws, int /*code*/, std::string_view /*message*/) {
            // /* You may access ws->getUserData() here, but sending or
            //  * doing any kind of I/O with the socket is not valid. */
            PerSocketData *socketData = ws->getUserData();

        }

    }).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });

    app.run();
}
