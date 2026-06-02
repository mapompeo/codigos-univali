package br.univali.cc.prog3.banco.dominio;

public class BancoException extends RuntimeException {

    public BancoException(String mensagem) {
        super(mensagem);
    }
}
