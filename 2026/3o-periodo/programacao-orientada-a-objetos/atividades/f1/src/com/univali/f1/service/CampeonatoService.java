
package com.univali.f1.service;

import com.univali.f1.model.*;
import java.util.Date;

public class CampeonatoService {

    private final Organizador organizador;
    private Campeonato campeonatoAtivo;

    public CampeonatoService(String nomeOrganizador) {
        this.organizador = new Organizador(nomeOrganizador);
    }

    public String criarCampeonato(int ano, String nome) {
        campeonatoAtivo = organizador.criarCampeonato(ano, nome);
        return "Campeonato \"" + campeonatoAtivo + "\" criado com sucesso.";
    }

    public String inscreverEquipe(String nomeEq,
            String nomeP1, int lic1,
            String nomeP2, int lic2,
            String... mecanicos) {
        if (campeonatoAtivo == null)
            return "Erro: nenhum campeonato ativo.";

        Piloto p1 = new Piloto(lic1, nomeP1);
        Piloto p2 = new Piloto(lic2, nomeP2);
        Carro c1 = new Carro(1, p1);
        Carro c2 = new Carro(2, p2);

        Equipe eq = new Equipe(nomeEq);
        eq.addPiloto(p1);
        eq.addPiloto(p2);
        eq.addCarro(c1);
        eq.addCarro(c2);
        for (String m : mecanicos)
            eq.addMecanico(new Mecanico(m));

        boolean ok = organizador.inscrever(campeonatoAtivo, eq);
        if (!ok)
            return "Erro: campeonato cheio (máx 12 equipes) ou equipe incompleta.";
        return "Equipe \"" + nomeEq + "\" inscrita! Vagas restantes: " + (12 - campeonatoAtivo.getEquipes().size());
    }

    public String adicionarCorrida(String local) {
        if (campeonatoAtivo == null)
            return "Erro: nenhum campeonato ativo.";
        boolean ok = campeonatoAtivo.definirCorrida(new Autodromo(local), new Date());
        if (!ok)
            return "Erro: limite de 20 corridas atingido.";
        return "Corrida em \"" + local + "\" adicionada. Total: " + campeonatoAtivo.getCorridas().size() + "/20";
    }

    public Campeonato getCampeonatoAtivo() {
        return campeonatoAtivo;
    }

    public Organizador getOrganizador() {
        return organizador;
    }
}
