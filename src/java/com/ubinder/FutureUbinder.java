package com.ubinder;

import com.ubinder.Ubinder;

import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.CompletableFuture;
import java.util.function.BiFunction;
import java.util.function.Function;

public class FutureUbinder {

    public FutureUbinder(
        BiFunction<byte[], Function<byte[], Void>, Void> onRequest,
        Function<byte[], Void> onNotification) {
        _onRequest = onRequest;
        _ubinder = new Ubinder(
            (Long req, byte[] arr) -> this.OnRequest(req, arr),
            (Long req, byte[] arr) -> this.OnResponse(req, arr),
            onNotification);
        _sendedRequests = new HashMap<>();
        _random = new Random();
    }

    public CompletableFuture<byte[]> SendRequest(byte[] data) {
        CompletableFuture<byte[]> future = new CompletableFuture<>();
        long reqId = _random.nextLong();
        _sendedRequests.put(reqId, future);
        _ubinder.SendRequest(reqId, data);
        return future;
    }

    public void SendNotification(byte[] data) {
        _ubinder.SendNotification(data);
    }

    private Void OnRequest(Long req, byte[] arr) {
        return _onRequest.apply(
            arr,
            (responseData) -> {
                _ubinder.SendResponse(req, responseData);
                return null;
            });
    }

    public Void OnResponse(Long req, byte[] arr) {
        CompletableFuture<byte[]> future = _sendedRequests.get(req);
        if (future != null) {
            _sendedRequests.remove(req);
            future.complete(arr);
        }
        return null;
    }

    private Ubinder _ubinder;
    private HashMap<Long, CompletableFuture<byte[]>> _sendedRequests;
    private java.util.Random _random;
    private BiFunction<byte[], Function<byte[], Void>, Void> _onRequest;
}
