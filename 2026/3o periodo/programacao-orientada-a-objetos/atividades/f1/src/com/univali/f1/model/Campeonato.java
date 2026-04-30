
package com.univali.f1.model;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;

public class Campeonato {
    private final int ano;
    private final String nome;
    private final List<Corrida> corridas = new ArrayList<>();
    private final List<Equipe> equipes = new ArrayList<>();

    public Campeonato(int ano, String nome) {
        this.ano = ano;
        this.nome = nome;
    }

    public boolean definirCorrida(Autodromo autodromo, Date data) {
        if (corridas.size() >= 20)
            return false;
        corridas.add(new Corrida(data, autodromo));
        return true;
    }

    public boolean inscreverEquipe(Equipe equipe) {
        if (equipes.size() >= 12 || !equipe.isCompleta())
            return false;
        equipes.add(equipe);
        return true;
    }

    public int getAno() {
        return ano;
    }

    public String getNome() {
        return nome;
    }

    public List<Corrida> getCorridas() {
        return Collections.unmodifiableList(corridas);
    }

    public List<Equipe> getEquipes() {
        return Collections.unmodifiableList(equipes);
    }

    @Override
    public String toString() {
        return nome + " — " + ano;
    }
}
