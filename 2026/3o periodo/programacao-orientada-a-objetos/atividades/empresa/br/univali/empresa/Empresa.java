package br.univali.empresa;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
    public List<Pagavel> contas = new ArrayList<>();

    public void adicionarDespesa(Pagavel pagavel) {
        contas.add(pagavel);
    }

    public float calcularTotalDeGastos() {
        float total = 0;
        for (Pagavel p : contas) {
            total += p.getValor();
        }
        return total;
    }
}
