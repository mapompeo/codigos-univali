
package com.univali.f1.model;

public class Autodromo {
    private final String local;

    public Autodromo(String local) {
        this.local = local;
    }

    public String getLocal() {
        return local;
    }

    @Override
    public String toString() {
        return local;
    }
}
