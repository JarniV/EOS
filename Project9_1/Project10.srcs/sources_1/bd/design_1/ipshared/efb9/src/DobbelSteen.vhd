library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity DobbelSteen is
    Port ( inn : in STD_LOGIC_VECTOR (3 downto 0);
           uit : out STD_LOGIC_VECTOR (7 downto 0));

end DobbelSteen;

architecture Behavioral of DobbelSteen is

begin

process(inn)
begin
    case inn is
        when "0001" => uit <= "00010000";
        when "0010" => uit <= "10000001";
        when "0011" => uit <= "10010001";
        when "0100" => uit <= "10100101";
        when "0101" => uit <= "10110101";
        when "0110" => uit <= "11100111";
        when others => uit <= "00000000";
    end case;
end process;

end Behavioral;
