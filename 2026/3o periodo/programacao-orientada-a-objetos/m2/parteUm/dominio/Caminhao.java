package parteUm.dominio;

public class Caminhao extends VeiculoTerrestre {
    private double capcacidadeCarga;

    public Caminhao(String modelo, String placa, double capcacidadeCarga) {
        super(modelo, placa);
        this.capcacidadeCarga = capcacidadeCarga;
    }

    public double getCapacidadeCarga() {
        return capcacidadeCarga;
    }
}
