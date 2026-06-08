package parteDois.dominio;

public class Caminhao extends VeiculoTerrestre {
    private double capacidadeCarga;

    public Caminhao(String modelo, String placa, double capacidadeCarga) {
        super(modelo, placa);
        this.capacidadeCarga = capacidadeCarga;
    }

    public double getCapacidadeCarga() {
        return capacidadeCarga;
    }

    @Override
    public String obterDescricao() {
        return super.detalhar() + ", Capacidade de carga: " + capacidadeCarga;
    }
}
