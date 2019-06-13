key_store_node = require('bindings')('key_store_node')
messages = require('./generated/messages_pb')

function Callbacker(wrapperLib) {
    
    function OnRequest(req, data) {
    
    }

    function OnResponse(req, data) {
        console.log(req);    
    }

    function OnNotification(data) {
    
    }
    this.lib = wrapperLib;
    this.lib.registerLib(OnRequest, OnResponse, OnNotification);

    this.sendRequest = function (req, callback) {
        var reqId = new Uint8Array(8);
        reqId[0] = 16;
        this.lib.sendRequest(reqId, req);
    }
}


callbacker = new Callbacker(key_store_node)

createAddValue = function(key, value) {
    var addVal = new messages.AddValueRequest();
    addVal.setKey(key);
    addVal.setValue(value);
    var req = new messages.Request();
    req.setRequestType(messages.RequestType.ADD_VALUE);
    req.setRequestBody(addVal.serializeBinary());
    return req;
}

createGetValue = function (key) {
    var getVal = new messages.GetValueRequest();
    getVal.setKey(key);
    var req = new messages.Request();
    req.setRequestType(messages.RequestType.GET_VALUE);
    req.setRequestBody(getVal.serializeBinary());
    return req;
}

sendAddValue = function(key, value) {
    return new Promise((resolve, reject) => {
        var msg = createAddValue(key, value).serializeBinary()
        callbacker.sendRequest(msg, 
            function(data){
                var resp = messages.AddValueResponse.deserializeBinary(data);
                if (resp.getError()=='') {
                    resolve();
                }else {
                    reject(new Error(resp.getError()));
                }
            });        
        });
}

sendGetValue = function(key) {
    return new Promise((resolve, reject) => {
        var msg = createGetValue(key).serializeBinary()
        callbacker.sendRequest(msg, 
            function(data){
                var resp = messages.GetValueResponse.deserializeBinary(data);
                if (resp.getError()=='') {
                    resolve(resp.getValue());
                }else {
                    reject(new Error(resp.getError()));
                }
            });
    });
}


run_test = function() {
    sendAddValue("key1", "value1")
        .then(() => sendGetValue("key1"))
        .then((val) => console.log(val));
    sendAddValue("key2", "value2")
        .then(() => sendGetValue("key2"))
        .then((val) => console.log(val));
    sendAddValue("key3", "value3")
        .then(() => sendGetValue("key3"))
        .then((val) => console.log(val));
}
