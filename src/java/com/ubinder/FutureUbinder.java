package com.ubinder;

import com.ubinder.Ubinder;

import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.CompletableFuture;
import java.util.function.BiConsumer;
import java.util.function.Consumer;

public class FutureUbinder {

    public FutureUbinder(
        final BiConsumer<byte[], Consumer<byte[]>> onRequest,
        final Consumer<byte[]> onNotification) {
        _ubinder = new Ubinder(
                new BiConsumer<Integer, byte[]>() {
                    @Override
                    public void accept(final Integer req, byte[] arr) {
                        onRequest.accept(
                                arr,
                                new Consumer<byte[]>() {
                                    @Override
                                    public void accept(byte[] responseData) {
                                        _ubinder.SendResponse(req, responseData);
                                    }
                                });
                    }
                },
                new BiConsumer<Integer, byte[]>() {
                    @Override
                    public void accept(Integer req, byte[] arr) {
                        CompletableFuture<byte[]> future = _sendedRequests.get(req);
                        if (future != null) {
                            _sendedRequests.remove(req);
                            future.complete(arr);
                        }
                    }
                },
                onNotification
        );
        _sendedRequests = new HashMap<>();
        _random = new Random();
    }

    public CompletableFuture<byte[]> SendRequest(byte[] data) {
        CompletableFuture<byte[]> future = new CompletableFuture<>();
        int reqId = _random.nextInt();
        _sendedRequests.put(reqId, future);
        _ubinder.SendRequest(reqId, data);
        return future;
    }

    private Ubinder _ubinder;
    private HashMap<Integer, CompletableFuture<byte[]>> _sendedRequests;
    private java.util.Random _random;
}
