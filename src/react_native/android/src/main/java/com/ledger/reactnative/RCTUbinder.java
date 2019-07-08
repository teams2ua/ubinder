package com.ledger.reactnative;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.ubinder.Ubinder;

import android.util.Base64;

import java.util.function.BiConsumer;
import java.util.function.Consumer;


public class RCTUbinder extends ReactContextBaseJavaModule {

    // JNI class that represent binding
    private Ubinder _ubinder;

    // There are limitation in React Native <-> Native Module communication
    // binary type is not supported, so we work with base64
    public RCTUbinder(ReactApplicationContext reactContext)
    {
        super(reactContext);
        _ubinder = new Ubinder(
                new BiConsumer<Integer, byte[]>() {
                    @Override
                    public void accept(final Integer req, byte[] arr) {
                        WritableMap params = Arguments.createMap();
                        params.putInt("requestId", req);
                        params.putString("data", Base64.encodeToString(arr, Base64.DEFAULT));
                        sendEvent("onRequest", params);
                    }
                },
                new BiConsumer<Integer, byte[]>() {
                    @Override
                    public void accept(final Integer req, byte[] arr) {
                        WritableMap params = Arguments.createMap();
                        params.putInt("requestId", req);
                        params.putString("data", Base64.encodeToString(arr, Base64.DEFAULT));
                        sendEvent("onResponse", params);
                    }
                },
                new Consumer<byte[]>() {
                    @Override
                    public void accept(byte[] arr) {
                        WritableMap params = Arguments.createMap();
                        params.putString("data", Base64.encodeToString(arr, Base64.DEFAULT));
                        sendEvent("onNotification", params);
                    }
                });
    }

    private void sendEvent(String eventName,
                           @Nullable WritableMap params) {
        getReactApplicationContext()
                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                .emit(eventName, params);
    }

    @Override
    public String getName()
    {
        return "RCTUbinder";
    }

    @ReactMethod
    public void SendRequest(Integer reqId, String data)
    {
        byte[] decodedData = Base64.decode(data, Base64.DEFAULT);
        _ubinder.SendRequest(reqId, decodedData);
    }

    @ReactMethod
    public void SendResponse(Integer reqId, String data)
    {
        byte[] decodedData = Base64.decode(data, Base64.DEFAULT);
        _ubinder.SendResponse(reqId, decodedData);
    }

    @ReactMethod
    public void SendNotification(String data)
    {
        byte[] decodedData = Base64.decode(data, Base64.DEFAULT);
        _ubinder.SendNotification(decodedData);
    }
}
