// Promise based interface of ubinder (to hide request Id routines from client)
module.exports.UbinderOnPromises = class UbinderOnPromises {
    constructor(rawUbinder, onNotification, onRequest, onExit) {
        this.pendingRequests = new Map();
        this.lib = rawUbinder;
        this.onNotification = onNotification;
        this.onRequest = onRequest;
        this.lib.registerLib(
            (reqId, data)=>{this.OnRequest(reqId, data)},
            (reqId, data)=>{this.OnResponse(reqId, data)},
            onNotification,
            onExit);

        this.OnResponse = function(reqId, data) {
            var value = this.pendingRequests.get(reqId);
            this.pendingRequests.delete(reqId);
            value.resolve(data);
        }
        this.OnRequest = function(reqId, data) {
            var sendFunction = this.lib.sendResponse;
            this.onRequest(data, function (responseData) { sendFunction(reqId, responseData); });
        }

        this.sendRequest = function (data) {
            var pr = new Promise((resolve, reject) => {
                var reqId = Math.floor(Math.random() * 2147483647);
                this.pendingRequests.set(reqId, {'resolve':resolve, 'reject':reject});
                this.lib.sendRequest(reqId, data);
            });
            return pr;
        };
        this.sendNotification = function (data) {
            this.lib.sendNotification(data);
        };

        this.exit = this.lib.exit;
    }
}
