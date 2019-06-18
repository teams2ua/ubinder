package com.ubinder;

import java.util.function.BiConsumer;
import java.util.function.Consumer;

public class Ubinder {
    private static native void ubinderInit();

    static {
        System.loadLibrary("ubinder_jni");
        ubinderInit();
    }

    public native void SendRequest(int req, byte[] arr);
    public native void SendResponse(int req, byte[] arr);
    public native void SendNotification(byte[] arr);

    public void OnRequest(int req, byte[] arr) {
         _onRequest.accept(req, arr);
    }

    public void OnResponse(int req, byte[] arr) {
         _onResponse.accept(req, arr);
    }

    public void OnNotification(byte[] arr) { _onNotification.accept(arr); }

    public Ubinder(BiConsumer<Integer, byte[]> onRequest, BiConsumer<Integer, byte[]> onResponse, Consumer<byte[]> onNotification) {
        _onRequest = onRequest;
        _onResponse = onResponse;
        _onNotification = onNotification;
    }
 
    private BiConsumer<Long, byte[]> _onRequest;
    private BiConsumer<Long, byte[]> _onResponse;
    private Consumer<byte[]> _onNotification;
}