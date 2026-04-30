
package com.univali.f1.model;

public class Mecanico {
    private final String nome;

    public Mecanico(String nome) {
        this.nome = nome;
    }

    public String getNome() {
        return nome;
    }

    @Override
    public String toString() {
        return nome;
    }
}
