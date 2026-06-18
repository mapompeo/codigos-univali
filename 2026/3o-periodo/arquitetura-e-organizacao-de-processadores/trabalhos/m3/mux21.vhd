-- Code your design here
library IEEE;
use IEEE.std_logic_1164.all;
--desenho em vermelho
entity mux21 is
    port(a,b : in std_logic_vector(31 downto 0);
         sel : in std_logic;
         s : out std_logic_vector(31 downto 0));
end mux21;

--desenho em preto
architecture arch of mux21 is


begin
    process (a, b, sel)
    begin
        if (sel = '0') then
            s <= a;
        else
            s <= b;
        end if;
    end process;


end arch;
