-- Code your testbench here
library IEEE;
use IEEE.std_logic_1164.all;

entity testbench is
end testbench;

architecture arch of testbench is
    signal STOP   : BOOLEAN;
    constant PERIOD : TIME := 10 NS;
    signal w_CLK  : std_logic := '0';
    signal w_RST  : std_logic := '0';

    -- saídas de debug do processador
    signal w_pc   : std_logic_vector(31 downto 0);
    signal w_ula  : std_logic_vector(31 downto 0);
    signal w_zero : std_logic;

begin

    DUT: entity work.design
    port map(w_CLK, w_RST, w_pc, w_ula, w_zero);

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
        STOP <= FALSE;

        -- pulso de reset: garante PC=0 no início
        w_RST <= '1';
        wait for PERIOD;
        w_RST <= '0';

        -- =====================================================================
        -- Resultados esperados no waveform (o_ula e o_pc):
        --
        --  ciclo  o_pc   instrução              o_ula  registrador escrito
        --  -----  -----  ---------------------  -----  -------------------
        --    1     0     ADDI x5,  x0,  6         6    x5  = 6
        --    2     4     ADDI x6,  x0,  4         4    x6  = 4
        --    3     8     ADD  x7,  x5,  x6        10   x7  = 10
        --    4    12     SUB  x8,  x7,  x6         6   x8  = 6
        --    5    16     AND  x9,  x5,  x6         4   x9  = 4  (6&4=4)
        --    6    20     OR   x10, x5,  x6         6   x10 = 6  (6|4=6)
        --    7    24     SW   x5,  0(x0)           0   mem[0] = 6
        --    8    28     LW   x11, 0(x0)           0   x11 = 6 (via mem)
        --    9    32     BEQ  x5,  x11, +8         0   branch tomado → PC=40
        --   10    40     ADDI x13, x0,  1          1   x13 = 1
        --   11    44     NOP                       0
        --
        -- PC=36 (ADDI x12,x0,99) NÃO aparece → confirma que branch funcionou
        -- =====================================================================

        wait for PERIOD * 13;   -- cobre os 11 ciclos de instrução + margem

        STOP <= TRUE;
        wait;
    end process;

end arch;
