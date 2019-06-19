key_store_node = require('bindings')('key_store_node')
ubinder = require('../../../src/node/ubinder');
messages = require('./generated/messages_pb')

function OnRequest(data, callback) {
    console.log("OnRequest");
}

function OnNotification(data) {
    console.log("OnNotification");
}


callbacker = new ubinder.Callbacker(key_store_node, OnRequest, OnNotification)

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

async function sendAddValue(key, value) {
        var msg = createAddValue(key, value).serializeBinary();
        var data = await callbacker.sendRequest(msg);
        var resp = messages.AddValueResponse.deserializeBinary(data);
        if (resp.getError()!='')
            throw new Error(resp.getError())
}

async function sendGetValue(key) {
    var msg = createGetValue(key).serializeBinary();
    var data = await callbacker.sendRequest(msg);
    var resp = messages.GetValueResponse.deserializeBinary(data);
    if (resp.getError()=='')
        return resp.getValue();
    throw new Error(resp.getError());
}


run_test = function() {
    sendAddValue("key1", "value1")
        .then(() => sendGetValue("key1"))
        .then((val) => console.log(val))
        .catch((x)=> console.log(x));
    sendAddValue("key2", "value2")
        .then(() => sendGetValue("key2"))
        .then((val) => console.log(val))
        .catch((x)=> console.log(x));
    sendAddValue("key3", "value3")
        .then(() => sendGetValue("key3"))
        .then((val) => console.log(val))
        .catch((x)=> console.log(x));
}
