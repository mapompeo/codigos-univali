-- Code your testbench here
library IEEE;
use IEEE.std_logic_1164.all;

entity testbench is
end testbench;

architecture arch of testbench is
    -- sinais do testbench
    signal STOP  : BOOLEAN;
    constant PERIOD: TIME := 10 NS;
    signal w_CLK : std_logic := '0';


    signal w_inst     : std_logic_vector(31 downto 0);
    signal w_ula_op: std_logic_vector( 3 downto 0);
    signal w_sel   : std_logic; --seleciona entre b e imm
    signal w_ula   : std_logic_vector(31 downto 0);
    signal w_zero  : std_logic;



begin

    DUT: entity work.design
    port map(w_clk, w_inst, w_ula_op, w_sel, w_ula, w_zero );

    -- Gerador de CLOCK
    u_CLK_GEN: process
    begin
      while not STOP loop
              w_CLK <= '0';
            wait for PERIOD/2;
            w_CLK <= '1';
              wait for PERIOD/2;
      end loop;
      wait;
    end process u_CLK_GEN;



    process
    begin
        -- INÍCIO DA SIMULAÇÃO (todas as entradas em zero)
             STOP <= FALSE;





        w_inst     <="00000000011000000000001010010011"; --instrução
        w_ula_op<="0000"; --soma
        w_sel  <= '1'; --seleciona imm
        wait for PERIOD;

        w_inst     <="00000000010000000000001100010011"; --instrução
        w_ula_op<="0000"; --soma
        w_sel  <= '1'; --seleciona imm
        wait for PERIOD;

        w_inst     <="00000000011000101000001110110011"; --instrução
        w_ula_op<="0000"; --soma
        w_sel  <= '0'; --seleciona imm
        wait for PERIOD;




        -- FIM DA SIMULAÇÃO
          STOP <= TRUE;
        wait;

    end process;

end arch;
