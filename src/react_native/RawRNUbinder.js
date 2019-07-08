import { NativeEventEmitter, NativeModules } from 'react-native';
import { Buffer } from 'buffer'

export default class RawRNUbinder {
    constructor () {
        this.ubinderEmitter = new NativeEventEmitter(NativeModules.Ubinder);
    }

    registerLib(OnRequest, OnResponse, OnNotification) {
        this.onRequestSub = this.ubinderEmitter.addListener('onRequest', function(e) {
            OnRequest(e.requestId, new Uint8Array(Buffer.from(e.data, "base64")));
        });
    
        this.onResponseSub = this.ubinderEmitter.addListener('onResponse', function(e) {
            OnResponse(e.requestId, new Uint8Array(Buffer.from(e.data, "base64")));
        });
    
        this.onNotificationSub = this.ubinderEmitter.addListener('onNotification', function(e) {
            OnNotification(new Uint8Array(Buffer.from(e.data, "base64")));
        });
    }

    sendRequest(reqId, data) {
        var strB64 = Buffer.from(data).toString("base64");
        NativeModules.Ubinder.SendRequest(reqId, strB64);
    }

    sendResponse(reqId, data) {
        NativeModules.Ubinder.SendResponse(reqId, Buffer.from(data).toString("base64"));
    }

    sendNotification(data) {
        NativeModules.Ubinder.SendNotification(Buffer.from(data).toString("base64"));
    }
    
    unmount() {
        this.onRequestSub.remove();
        this.onResponseSub.remove();
        this.onNotificationSub.remove();
    }   
}

