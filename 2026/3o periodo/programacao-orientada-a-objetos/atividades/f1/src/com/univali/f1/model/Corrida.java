
package com.univali.f1.model;

import java.util.Date;

public class Corrida {
    private final Date data;
    private final Autodromo autodromo;

    Corrida(Date data, Autodromo autodromo) {
        this.data = data;
        this.autodromo = autodromo;
    }

    public Date getData() {
        return data;
    }

    public Autodromo getAutodromo() {
        return autodromo;
    }

    @Override
    public String toString() {
        return autodromo.getLocal() + " — " + data;
    }
}
