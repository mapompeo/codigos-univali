
package com.univali.f1.model;

import java.util.ArrayList;
import java.util.List;

public class Equipe {
    private final String nome;
    private final List<Piloto> pilotos = new ArrayList<>();
    private final List<Carro> carros = new ArrayList<>();
    private final List<Mecanico> mecanicos = new ArrayList<>();

    public Equipe(String nome) {
        this.nome = nome;
    }

    public boolean addPiloto(Piloto p) {
        if (pilotos.size() >= 2)
            return false;
        pilotos.add(p);
        return true;
    }

    public boolean addCarro(Carro c) {
        if (carros.size() >= 2)
            return false;
        carros.add(c);
        return true;
    }

    public void addMecanico(Mecanico m) {
        mecanicos.add(m);
    }

    public boolean isCompleta() {
        return pilotos.size() == 2 && carros.size() == 2;
    }

    public String getNome() {
        return nome;
    }

    public List<Piloto> getPilotos() {
        return pilotos;
    }

    public List<Carro> getCarros() {
        return carros;
    }

    public List<Mecanico> getMecanicos() {
        return mecanicos;
    }

    @Override
    public String toString() {
        return nome;
    }
}
