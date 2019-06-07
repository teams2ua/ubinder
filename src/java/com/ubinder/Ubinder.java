package com.ubinder;

import java.util.function.BiFunction;
import java.util.function.Function;

public class Ubinder {
    private static native void ubinderInit();

    static {
        System.loadLibrary("ubinder_jni");
        ubinderInit();
    }

    public native void SendRequest(long req, byte[] arr);
    public native void SendResponse(long req, byte[] arr);
    public native void SendNotification(byte[] arr);

    public void OnRequest(long req, byte[] arr) {
         _onRequest.apply(req, arr);
    }

    public void OnResponse(long req, byte[] arr) {
         _onResponse.apply(req, arr);
    }

    public void OnNotification(byte[] arr) {
        _onNotification.apply(arr);
    }

    public Ubinder(BiFunction<Long, byte[], Void> onRequest, BiFunction<Long, byte[], Void> onResponse, Function<byte[], Void> onNotification) {
        _onRequest = onRequest;
        _onResponse = onResponse;
        _onNotification = onNotification;
    }
 
    private BiFunction<Long, byte[], Void> _onRequest;
    private BiFunction<Long, byte[], Void> _onResponse;
    private Function<byte[], Void> _onNotification;
}