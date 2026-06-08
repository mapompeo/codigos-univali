
package com.univali.f1.model;

public class Piloto {
    private final int licenca;
    private final String nome;

    public Piloto(int licenca, String nome) {
        this.licenca = licenca;
        this.nome = nome;
    }

    public int getLicenca() {
        return licenca;
    }

    public String getNome() {
        return nome;
    }

    @Override
    public String toString() {
        return nome + " (Lic: " + licenca + ")";
    }
}
