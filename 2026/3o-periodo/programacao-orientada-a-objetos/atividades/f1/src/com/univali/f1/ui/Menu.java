
package com.univali.f1.ui;

import com.univali.f1.model.*;
import com.univali.f1.service.CampeonatoService;
import java.util.List;
import java.util.Scanner;

public class Menu {

    private final Scanner sc = new Scanner(System.in);
    private final CampeonatoService service = new CampeonatoService("FIA");

    public static void main(String[] args) {
        new Menu().iniciar();
    }

    private void iniciar() {
        limparTela();
        cabecalho();
        boolean rodando = true;
        while (rodando) {
            menuPrincipal();
            String opcao = sc.nextLine().trim();
            switch (opcao) {
                case "1" -> fluxoCriarCampeonato();
                case "2" -> fluxoInscreverEquipe();
                case "3" -> fluxoAdicionarCorrida();
                case "4" -> fluxoVerStatus();
                case "0" -> rodando = false;
                default -> erro("Opção inválida.");
            }
        }
        println("\nEncerrando sistema FIA... até logo!\n");
    }

    private void fluxoCriarCampeonato() {
        titulo("CRIAR CAMPEONATO");
        String nome = pedir("Nome do campeonato");
        int ano = pedirInt("Ano");
        sucesso(service.criarCampeonato(ano, nome));
        pausar();
    }

    private void fluxoInscreverEquipe() {
        if (service.getCampeonatoAtivo() == null) {
            erro("Crie um campeonato primeiro!");
            pausar();
            return;
        }

        titulo("INSCREVER EQUIPE");
        String nomeEq = pedir("Nome da equipe");
        println("Piloto 1");
        String nomeP1 = pedir("  Nome");
        int lic1 = pedirInt("  Licença");
        println("Piloto 2");
        String nomeP2 = pedir("  Nome");
        int lic2 = pedirInt("  Licença");
        println("Mecânicos (deixe em branco para encerrar)");

        java.util.List<String> mecs = new java.util.ArrayList<>();
        while (true) {
            String m = pedir("  Mecânico " + (mecs.size() + 1) + " (ou Enter para pular)");
            if (m.isEmpty())
                break;
            mecs.add(m);
        }

        String resultado = service.inscreverEquipe(nomeEq, nomeP1, lic1, nomeP2, lic2, mecs.toArray(String[]::new));
        if (resultado.startsWith("Erro"))
            erro(resultado);
        else
            sucesso(resultado);
        pausar();
    }

    private void fluxoAdicionarCorrida() {
        if (service.getCampeonatoAtivo() == null) {
            erro("Crie um campeonato primeiro!");
            pausar();
            return;
        }
        titulo("ADICIONAR CORRIDA");
        String local = pedir("Local do autódromo");
        String resultado = service.adicionarCorrida(local);
        if (resultado.startsWith("Erro"))
            erro(resultado);
        else
            sucesso(resultado);
        pausar();
    }

    private void fluxoVerStatus() {
        Campeonato c = service.getCampeonatoAtivo();
        if (c == null) {
            erro("Nenhum campeonato ativo.");
            pausar();
            return;
        }

        titulo("STATUS: " + c);

        println("Corridas: " + c.getCorridas().size() + "/20");
        int i = 1;
        for (Corrida corrida : c.getCorridas())
            println(i++ + ". " + corrida.getAutodromo().getLocal());
        if (c.getCorridas().isEmpty())
            println("Nenhuma corrida cadastrada.");

        println("");
        println("Equipes: " + c.getEquipes().size() + "/12");
        for (Equipe eq : c.getEquipes()) {
            println(eq.getNome());
            List<Piloto> ps = eq.getPilotos();
            for (int j = 0; j < ps.size(); j++)
                println("  Carro " + (j + 1) + ": " + ps.get(j).getNome() + " (lic: " + ps.get(j).getLicenca() + ")");
        }
        if (c.getEquipes().isEmpty())
            println("Nenhuma equipe inscrita.");
        pausar();
    }

    private void cabecalho() {
        println("SISTEMA DE CAMPEONATO DE FÓRMULA 1");
        println("Federação Internacional - FIA");
        println("");
    }

    private void menuPrincipal() {
        Campeonato c = service.getCampeonatoAtivo();
        String campInfo = (c == null)
                ? "nenhum campeonato ativo"
                : c.toString();
        println("\n" + campInfo);
        println("[1] Criar campeonato");
        println("[2] Inscrever equipe");
        println("[3] Adicionar corrida");
        println("[4] Ver status");
        println("[0] Sair");
        print("\n> ");
    }

    private void titulo(String t) {
        println("\n" + t);
    }

    private void sucesso(String msg) {
        println("OK: " + msg);
    }

    private void erro(String msg) {
        println("ERRO: " + msg);
    }

    private String pedir(String label) {
        print(label + ": ");
        return sc.nextLine().trim();
    }

    private int pedirInt(String label) {
        while (true) {
            try {
                return Integer.parseInt(pedir(label));
            } catch (NumberFormatException e) {
                erro("Digite um número inteiro.");
            }
        }
    }

    private void pausar() {
        print("\nPressione Enter para continuar");
        sc.nextLine();
        limparTela();
        cabecalho();
    }

    private void limparTela() {
        System.out.println("\n\n");
    }

    private void println(String s) {
        System.out.println(s);
    }

    private void print(String s) {
        System.out.print(s);
        System.out.flush();
    }
}
