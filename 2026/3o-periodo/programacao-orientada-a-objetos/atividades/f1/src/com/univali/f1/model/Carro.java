
package com.univali.f1.model;

public class Carro {
    private final int numero;
    private final Piloto piloto;

    public Carro(int numero, Piloto piloto) {
        this.numero = numero;
        this.piloto = piloto;
    }

    public int getNumero() {
        return numero;
    }

    public Piloto getPiloto() {
        return piloto;
    }

    @Override
    public String toString() {
        return "#" + numero + " > " + piloto.getNome();
    }
}
