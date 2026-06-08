
package com.univali.f1.model;

import java.util.ArrayList;
import java.util.List;

public class Organizador {
    private final String nome;
    private final List<Campeonato> campeonatos = new ArrayList<>();

    public Organizador(String nome) {
        this.nome = nome;
    }

    public Campeonato criarCampeonato(int ano, String nomeCamp) {
        Campeonato c = new Campeonato(ano, nomeCamp);
        campeonatos.add(c);
        return c;
    }

    public boolean inscrever(Campeonato campeonato, Equipe equipe) {
        return campeonato.inscreverEquipe(equipe);
    }

    public String getNome() {
        return nome;
    }

    public List<Campeonato> getCampeonatos() {
        return campeonatos;
    }

    @Override
    public String toString() {
        return nome;
    }
}
