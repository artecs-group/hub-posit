--------------------------------------------------------------------------------
--                       Normalizer_ZO_15_15_15_F0_uid6
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X OZb
-- Output signals: Count R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Normalizer_ZO_15_15_15_F0_uid6 is
    port (X : in  std_logic_vector(14 downto 0);
          OZb : in  std_logic;
          Count : out  std_logic_vector(3 downto 0);
          R : out  std_logic_vector(14 downto 0)   );
end entity;

architecture arch of Normalizer_ZO_15_15_15_F0_uid6 is
signal level4 :  std_logic_vector(14 downto 0);
signal sozb :  std_logic;
signal count3 :  std_logic;
signal level3 :  std_logic_vector(14 downto 0);
signal count2 :  std_logic;
signal level2 :  std_logic_vector(14 downto 0);
signal count1 :  std_logic;
signal level1 :  std_logic_vector(14 downto 0);
signal count0 :  std_logic;
signal level0 :  std_logic_vector(14 downto 0);
signal sCount :  std_logic_vector(3 downto 0);
begin
   level4 <= X ;
   sozb<= OZb;
   count3<= '1' when level4(14 downto 7) = (14 downto 7=>sozb) else '0';
   level3<= level4(14 downto 0) when count3='0' else level4(6 downto 0) & (7 downto 0 => '0');

   count2<= '1' when level3(14 downto 11) = (14 downto 11=>sozb) else '0';
   level2<= level3(14 downto 0) when count2='0' else level3(10 downto 0) & (3 downto 0 => '0');

   count1<= '1' when level2(14 downto 13) = (14 downto 13=>sozb) else '0';
   level1<= level2(14 downto 0) when count1='0' else level2(12 downto 0) & (1 downto 0 => '0');

   count0<= '1' when level1(14 downto 14) = (14 downto 14=>sozb) else '0';
   level0<= level1(14 downto 0) when count0='0' else level1(13 downto 0) & (0 downto 0 => '0');

   R <= level0;
   sCount <= count3 & count2 & count1 & count0;
   Count <= sCount;
end architecture;

--------------------------------------------------------------------------------
--                      PositHUBFastDecoder_16_2_F0_uid4
-- Version: 2022.10.20 - 101427
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Raul Murillo (2021-2022)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X
-- Output signals: Sign SF Frac NZN

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity PositHUBFastDecoder_16_2_F0_uid4 is
    port (X : in  std_logic_vector(15 downto 0);
          Sign : out  std_logic;
          SF : out  std_logic_vector(6 downto 0);
          Frac : out  std_logic_vector(11 downto 0);
          NZN : out  std_logic   );
end entity;

architecture arch of PositHUBFastDecoder_16_2_F0_uid4 is
   component Normalizer_ZO_15_15_15_F0_uid6 is
      port ( X : in  std_logic_vector(14 downto 0);
             OZb : in  std_logic;
             Count : out  std_logic_vector(3 downto 0);
             R : out  std_logic_vector(14 downto 0)   );
   end component;

signal sgn :  std_logic;
signal pNZN :  std_logic;
signal rc :  std_logic;
signal regPosit :  std_logic_vector(14 downto 0);
signal regLength :  std_logic_vector(3 downto 0);
signal shiftedPosit :  std_logic_vector(14 downto 0);
signal k :  std_logic_vector(4 downto 0);
signal sgnVect :  std_logic_vector(1 downto 0);
signal exp :  std_logic_vector(1 downto 0);
signal pSF :  std_logic_vector(6 downto 0);
signal pFrac :  std_logic_vector(11 downto 0);
begin
--------------------------- Start of vhdl generation ---------------------------
--------------------------- Sign bit & special cases ---------------------------
   sgn <= X(15);
   pNZN <= '0' when (X(14 downto 0) = "000000000000000") else '1';
-------------- Count leading zeros/ones of regime & shift it out --------------
   rc <= X(14);
   regPosit <= X(13 downto 0) & '1';   -- Introduces the implicit LSB for HUB format
   RegimeCounter: Normalizer_ZO_15_15_15_F0_uid6
      port map ( OZb => rc,
                 X => regPosit,
                 Count => regLength,
                 R => shiftedPosit);
----------------- Determine the scaling factor - regime & exp -----------------
   k <= "0" & regLength when rc /= sgn else "1" & NOT(regLength);
   sgnVect <= (others => sgn);
   exp <= shiftedPosit(13 downto 12) XOR sgnVect;
   pSF <= k & exp;
------------------------------- Extract fraction -------------------------------
   pFrac <= shiftedPosit(11 downto 0);
   Sign <= sgn;
   SF <= pSF;
   Frac <= pFrac;
   NZN <= pNZN;
---------------------------- End of vhdl generation ----------------------------
end architecture;

--------------------------------------------------------------------------------
--                      Normalizer_ZO_15_15_15_F0_uid10
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X OZb
-- Output signals: Count R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Normalizer_ZO_15_15_15_F0_uid10 is
    port (X : in  std_logic_vector(14 downto 0);
          OZb : in  std_logic;
          Count : out  std_logic_vector(3 downto 0);
          R : out  std_logic_vector(14 downto 0)   );
end entity;

architecture arch of Normalizer_ZO_15_15_15_F0_uid10 is
signal level4 :  std_logic_vector(14 downto 0);
signal sozb :  std_logic;
signal count3 :  std_logic;
signal level3 :  std_logic_vector(14 downto 0);
signal count2 :  std_logic;
signal level2 :  std_logic_vector(14 downto 0);
signal count1 :  std_logic;
signal level1 :  std_logic_vector(14 downto 0);
signal count0 :  std_logic;
signal level0 :  std_logic_vector(14 downto 0);
signal sCount :  std_logic_vector(3 downto 0);
begin
   level4 <= X ;
   sozb<= OZb;
   count3<= '1' when level4(14 downto 7) = (14 downto 7=>sozb) else '0';
   level3<= level4(14 downto 0) when count3='0' else level4(6 downto 0) & (7 downto 0 => '0');

   count2<= '1' when level3(14 downto 11) = (14 downto 11=>sozb) else '0';
   level2<= level3(14 downto 0) when count2='0' else level3(10 downto 0) & (3 downto 0 => '0');

   count1<= '1' when level2(14 downto 13) = (14 downto 13=>sozb) else '0';
   level1<= level2(14 downto 0) when count1='0' else level2(12 downto 0) & (1 downto 0 => '0');

   count0<= '1' when level1(14 downto 14) = (14 downto 14=>sozb) else '0';
   level0<= level1(14 downto 0) when count0='0' else level1(13 downto 0) & (0 downto 0 => '0');

   R <= level0;
   sCount <= count3 & count2 & count1 & count0;
   Count <= sCount;
end architecture;

--------------------------------------------------------------------------------
--                      PositHUBFastDecoder_16_2_F0_uid8
-- Version: 2022.10.20 - 101427
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Raul Murillo (2021-2022)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X
-- Output signals: Sign SF Frac NZN

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity PositHUBFastDecoder_16_2_F0_uid8 is
    port (X : in  std_logic_vector(15 downto 0);
          Sign : out  std_logic;
          SF : out  std_logic_vector(6 downto 0);
          Frac : out  std_logic_vector(11 downto 0);
          NZN : out  std_logic   );
end entity;

architecture arch of PositHUBFastDecoder_16_2_F0_uid8 is
   component Normalizer_ZO_15_15_15_F0_uid10 is
      port ( X : in  std_logic_vector(14 downto 0);
             OZb : in  std_logic;
             Count : out  std_logic_vector(3 downto 0);
             R : out  std_logic_vector(14 downto 0)   );
   end component;

signal sgn :  std_logic;
signal pNZN :  std_logic;
signal rc :  std_logic;
signal regPosit :  std_logic_vector(14 downto 0);
signal regLength :  std_logic_vector(3 downto 0);
signal shiftedPosit :  std_logic_vector(14 downto 0);
signal k :  std_logic_vector(4 downto 0);
signal sgnVect :  std_logic_vector(1 downto 0);
signal exp :  std_logic_vector(1 downto 0);
signal pSF :  std_logic_vector(6 downto 0);
signal pFrac :  std_logic_vector(11 downto 0);
begin
--------------------------- Start of vhdl generation ---------------------------
--------------------------- Sign bit & special cases ---------------------------
   sgn <= X(15);
   pNZN <= '0' when (X(14 downto 0) = "000000000000000") else '1';
-------------- Count leading zeros/ones of regime & shift it out --------------
   rc <= X(14);
   regPosit <= X(13 downto 0) & '1';   -- Introduces the implicit LSB for HUB format
   RegimeCounter: Normalizer_ZO_15_15_15_F0_uid10
      port map ( OZb => rc,
                 X => regPosit,
                 Count => regLength,
                 R => shiftedPosit);
----------------- Determine the scaling factor - regime & exp -----------------
   k <= "0" & regLength when rc /= sgn else "1" & NOT(regLength);
   sgnVect <= (others => sgn);
   exp <= shiftedPosit(13 downto 12) XOR sgnVect;
   pSF <= k & exp;
------------------------------- Extract fraction -------------------------------
   pFrac <= shiftedPosit(11 downto 0);
   Sign <= sgn;
   SF <= pSF;
   Frac <= pFrac;
   NZN <= pNZN;
---------------------------- End of vhdl generation ----------------------------
end architecture;

--------------------------------------------------------------------------------
--                     RightShifter14_by_max_14_F0_uid12
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca (2008-2011), Florent de Dinechin (2008-2019)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X S padBit
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity RightShifter14_by_max_14_F0_uid12 is
    port (X : in  std_logic_vector(13 downto 0);
          S : in  std_logic_vector(3 downto 0);
          padBit : in  std_logic;
          R : out  std_logic_vector(13 downto 0)   );
end entity;

architecture arch of RightShifter14_by_max_14_F0_uid12 is
signal ps :  std_logic_vector(3 downto 0);
signal level0 :  std_logic_vector(13 downto 0);
signal level1 :  std_logic_vector(14 downto 0);
signal level2 :  std_logic_vector(16 downto 0);
signal level3 :  std_logic_vector(20 downto 0);
signal level4 :  std_logic_vector(28 downto 0);
begin
   ps<= S;
   level0<= X;
   level1 <=  (0 downto 0 => padBit) & level0 when ps(0) = '1' else    level0 & (0 downto 0 => '0');
   level2 <=  (1 downto 0 => padBit) & level1 when ps(1) = '1' else    level1 & (1 downto 0 => '0');
   level3 <=  (3 downto 0 => padBit) & level2 when ps(2) = '1' else    level2 & (3 downto 0 => '0');
   level4 <=  (7 downto 0 => padBit) & level3 when ps(3) = '1' else    level3 & (7 downto 0 => '0');
   R <= level4(28 downto 15);
end architecture;

--------------------------------------------------------------------------------
--                      Normalizer_ZO_15_15_15_F0_uid14
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Florent de Dinechin, (2007-2020)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X OZb
-- Output signals: Count R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity Normalizer_ZO_15_15_15_F0_uid14 is
    port (X : in  std_logic_vector(14 downto 0);
          OZb : in  std_logic;
          Count : out  std_logic_vector(3 downto 0);
          R : out  std_logic_vector(14 downto 0)   );
end entity;

architecture arch of Normalizer_ZO_15_15_15_F0_uid14 is
signal level4 :  std_logic_vector(14 downto 0);
signal sozb :  std_logic;
signal count3 :  std_logic;
signal level3 :  std_logic_vector(14 downto 0);
signal count2 :  std_logic;
signal level2 :  std_logic_vector(14 downto 0);
signal count1 :  std_logic;
signal level1 :  std_logic_vector(14 downto 0);
signal count0 :  std_logic;
signal level0 :  std_logic_vector(14 downto 0);
signal sCount :  std_logic_vector(3 downto 0);
begin
   level4 <= X ;
   sozb<= OZb;
   count3<= '1' when level4(14 downto 7) = (14 downto 7=>sozb) else '0';
   level3<= level4(14 downto 0) when count3='0' else level4(6 downto 0) & (7 downto 0 => '0');

   count2<= '1' when level3(14 downto 11) = (14 downto 11=>sozb) else '0';
   level2<= level3(14 downto 0) when count2='0' else level3(10 downto 0) & (3 downto 0 => '0');

   count1<= '1' when level2(14 downto 13) = (14 downto 13=>sozb) else '0';
   level1<= level2(14 downto 0) when count1='0' else level2(12 downto 0) & (1 downto 0 => '0');

   count0<= '1' when level1(14 downto 14) = (14 downto 14=>sozb) else '0';
   level0<= level1(14 downto 0) when count0='0' else level1(13 downto 0) & (0 downto 0 => '0');

   R <= level0;
   sCount <= count3 & count2 & count1 & count0;
   Count <= sCount;
end architecture;

--------------------------------------------------------------------------------
--                     RightShifter14_by_max_14_F0_uid18
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Bogdan Pasca (2008-2011), Florent de Dinechin (2008-2019)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X S padBit
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity RightShifter14_by_max_14_F0_uid18 is
    port (X : in  std_logic_vector(13 downto 0);
          S : in  std_logic_vector(3 downto 0);
          padBit : in  std_logic;
          R : out  std_logic_vector(13 downto 0)   );
end entity;

architecture arch of RightShifter14_by_max_14_F0_uid18 is
signal ps :  std_logic_vector(3 downto 0);
signal level0 :  std_logic_vector(13 downto 0);
signal level1 :  std_logic_vector(14 downto 0);
signal level2 :  std_logic_vector(16 downto 0);
signal level3 :  std_logic_vector(20 downto 0);
signal level4 :  std_logic_vector(28 downto 0);
begin
   ps<= S;
   level0<= X;
   level1 <=  (0 downto 0 => padBit) & level0 when ps(0) = '1' else    level0 & (0 downto 0 => '0');
   level2 <=  (1 downto 0 => padBit) & level1 when ps(1) = '1' else    level1 & (1 downto 0 => '0');
   level3 <=  (3 downto 0 => padBit) & level2 when ps(2) = '1' else    level2 & (3 downto 0 => '0');
   level4 <=  (7 downto 0 => padBit) & level3 when ps(3) = '1' else    level3 & (7 downto 0 => '0');
   R <= level4(28 downto 15);
end architecture;

--------------------------------------------------------------------------------
--                     PositHUBFastEncoder_16_2_F0_uid16
-- Version: 2022.10.20 - 101427
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Raul Murillo (2021-2022)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: Sign SF Frac NZN
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity PositHUBFastEncoder_16_2_F0_uid16 is
    port (Sign : in  std_logic;
          SF : in  std_logic_vector(7 downto 0);
          Frac : in  std_logic_vector(10 downto 0);
          NZN : in  std_logic;
          R : out  std_logic_vector(15 downto 0)   );
end entity;

architecture arch of PositHUBFastEncoder_16_2_F0_uid16 is
   component RightShifter14_by_max_14_F0_uid18 is
      port ( X : in  std_logic_vector(13 downto 0);
             S : in  std_logic_vector(3 downto 0);
             padBit : in  std_logic;
             R : out  std_logic_vector(13 downto 0)   );
   end component;

signal rc :  std_logic;
signal rcVect :  std_logic_vector(4 downto 0);
signal k :  std_logic_vector(4 downto 0);
signal sgnVect :  std_logic_vector(1 downto 0);
signal exp :  std_logic_vector(1 downto 0);
signal ovf :  std_logic;
signal regValue :  std_logic_vector(3 downto 0);
signal regNeg :  std_logic;
signal padBit :  std_logic;
signal inputShifter :  std_logic_vector(13 downto 0);
signal shiftedPosit :  std_logic_vector(13 downto 0);
signal roundedPosit :  std_logic_vector(14 downto 0);
signal unsignedPosit :  std_logic_vector(14 downto 0);
begin
--------------------------- Start of vhdl generation ---------------------------
----------------------------- Get value of regime -----------------------------
   rc <= SF(SF'high);
   rcVect <= (others => rc);
   k <= SF(6 downto 2) XOR rcVect;
   sgnVect <= (others => Sign);
   exp <= SF(1 downto 0) XOR sgnVect;
   -- Check for regime overflow
   ovf <= '1' when (k > "01101") else '0';
   regValue <= k(3 downto 0) when ovf = '0' else "1110";
-------------- Generate regime - shift out exponent and fraction --------------
   regNeg <= Sign XOR rc;
   padBit <= NOT(regNeg);
   inputShifter <= regNeg & exp & Frac;
   RegimeGenerator: RightShifter14_by_max_14_F0_uid18
      port map ( S => regValue,
                 X => inputShifter,
                 padBit => padBit,
                 R => shiftedPosit);
   roundedPosit <= padBit & shiftedPosit(13 downto 1) & (shiftedPosit(0) OR ovf);   -- The LSB must be 1 if 'ovf' to deal with minpos/maxpos
-------------------------- Check sign & Special Cases --------------------------
   unsignedPosit <= roundedPosit when NZN = '1' else (others => '0');
   R <= Sign & unsignedPosit;
---------------------------- End of vhdl generation ----------------------------
end architecture;

--------------------------------------------------------------------------------
--                                 PositAdder
--                        (PositHUBAdder_16_2_F0_uid2)
-- Version: 2022.10.20 - 101427
-- VHDL generated for Kintex7 @ 0MHz
-- This operator is part of the Infinite Virtual Library FloPoCoLib
-- All rights reserved 
-- Authors: Raul Murillo (2021-2022)
--------------------------------------------------------------------------------
-- combinatorial
-- Clock period (ns): inf
-- Target frequency (MHz): 0
-- Input signals: X Y
-- Output signals: R

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity PositAdder is
    port (X : in  std_logic_vector(15 downto 0);
          Y : in  std_logic_vector(15 downto 0);
          R : out  std_logic_vector(15 downto 0)   );
end entity;

architecture arch of PositAdder is
   component PositHUBFastDecoder_16_2_F0_uid4 is
      port ( X : in  std_logic_vector(15 downto 0);
             Sign : out  std_logic;
             SF : out  std_logic_vector(6 downto 0);
             Frac : out  std_logic_vector(11 downto 0);
             NZN : out  std_logic   );
   end component;

   component PositHUBFastDecoder_16_2_F0_uid8 is
      port ( X : in  std_logic_vector(15 downto 0);
             Sign : out  std_logic;
             SF : out  std_logic_vector(6 downto 0);
             Frac : out  std_logic_vector(11 downto 0);
             NZN : out  std_logic   );
   end component;

   component RightShifter14_by_max_14_F0_uid12 is
      port ( X : in  std_logic_vector(13 downto 0);
             S : in  std_logic_vector(3 downto 0);
             padBit : in  std_logic;
             R : out  std_logic_vector(13 downto 0)   );
   end component;

   component Normalizer_ZO_15_15_15_F0_uid14 is
      port ( X : in  std_logic_vector(14 downto 0);
             OZb : in  std_logic;
             Count : out  std_logic_vector(3 downto 0);
             R : out  std_logic_vector(14 downto 0)   );
   end component;

   component PositHUBFastEncoder_16_2_F0_uid16 is
      port ( Sign : in  std_logic;
             SF : in  std_logic_vector(7 downto 0);
             Frac : in  std_logic_vector(10 downto 0);
             NZN : in  std_logic;
             R : out  std_logic_vector(15 downto 0)   );
   end component;

signal X_sgn :  std_logic;
signal X_sf :  std_logic_vector(6 downto 0);
signal X_f :  std_logic_vector(11 downto 0);
signal X_nzn :  std_logic;
signal Y_sgn :  std_logic;
signal Y_sf :  std_logic_vector(6 downto 0);
signal Y_f :  std_logic_vector(11 downto 0);
signal Y_nzn :  std_logic;
signal X_not_zero :  std_logic;
signal X_nar :  std_logic;
signal Y_not_zero :  std_logic;
signal Y_nar :  std_logic;
signal is_larger :  std_logic;
signal larger_sf :  std_logic_vector(6 downto 0);
signal smaller_sf :  std_logic_vector(6 downto 0);
signal larger_frac :  std_logic_vector(13 downto 0);
signal smaller_frac :  std_logic_vector(13 downto 0);
signal offset :  std_logic_vector(6 downto 0);
signal shift_saturate :  std_logic;
signal frac_offset :  std_logic_vector(3 downto 0);
signal input_shifter :  std_logic_vector(13 downto 0);
signal pad_bit :  std_logic;
signal shifted_frac :  std_logic_vector(13 downto 0);
signal smaller_frac_sh :  std_logic_vector(13 downto 0);
signal add_frac :  std_logic_vector(14 downto 0);
signal count_type :  std_logic;
signal add_frac_shift :  std_logic_vector(14 downto 0);
signal count :  std_logic_vector(3 downto 0);
signal norm_frac_tmp :  std_logic_vector(14 downto 0);
signal add_sf :  std_logic_vector(7 downto 0);
signal is_not_zero :  std_logic;
signal is_nar :  std_logic;
signal XY_nzn :  std_logic;
signal sign :  std_logic;
signal norm_frac :  std_logic_vector(10 downto 0);
begin
--------------------------- Start of vhdl generation ---------------------------
---------------------------- Decode X & Y operands ----------------------------
   X_decoder: PositHUBFastDecoder_16_2_F0_uid4
      port map ( X => X,
                 Frac => X_f,
                 NZN => X_nzn,
                 SF => X_sf,
                 Sign => X_sgn);
   Y_decoder: PositHUBFastDecoder_16_2_F0_uid8
      port map ( X => Y,
                 Frac => Y_f,
                 NZN => Y_nzn,
                 SF => Y_sf,
                 Sign => Y_sgn);
--------------------------- Check for Zeros and NaRs ---------------------------
   X_not_zero <= X_sgn OR X_nzn;
   X_nar <= X_sgn AND NOT(X_nzn);
   Y_not_zero <= Y_sgn OR Y_nzn;
   Y_nar <= Y_sgn AND NOT(Y_nzn);
---------------------- Compare operands and adjust values ----------------------
   is_larger <= '1' when (signed(X_sf) > signed(Y_sf)) else '0';
   with is_larger  select  larger_sf <= 
      X_sf when '1',
      Y_sf when '0',
      "-------" when others;
   with is_larger  select  smaller_sf <= 
      Y_sf when '1',
      X_sf when '0',
      "-------" when others;
   with is_larger  select  larger_frac <= 
      (X_sgn & (NOT(X_sgn) AND X_not_zero) & X_f) when '1',
      (Y_sgn & (NOT(Y_sgn) AND Y_not_zero) & Y_f) when '0',
      "--------------" when others;
   with is_larger  select  smaller_frac <= 
      (Y_sgn & (NOT(Y_sgn) AND Y_not_zero) & Y_f) when '1',
      (X_sgn & (NOT(X_sgn) AND X_not_zero) & X_f) when '0',
      "--------------" when others;
---------------- Compute exponents difference & align fractions ----------------
   offset <= larger_sf - smaller_sf;
   -- Saturate maximum offset
   shift_saturate <= '0' when (offset(6 downto 4) = "0000") else '1';
   frac_offset <= CONV_STD_LOGIC_VECTOR(15,4) when shift_saturate = '1' else offset(3 downto 0);
   -- Align fractions - right shift the smaller one
   input_shifter <= smaller_frac;
   pad_bit <= smaller_frac(smaller_frac'high) AND (NOT shift_saturate);
   RightShifterFraction: RightShifter14_by_max_14_F0_uid12
      port map ( S => frac_offset,
                 X => input_shifter,
                 padBit => pad_bit,
                 R => shifted_frac);
   smaller_frac_sh <= shifted_frac;
-------------------------------- Add fractions --------------------------------
   add_frac <= (larger_frac(larger_frac'high) & larger_frac) + (smaller_frac_sh(smaller_frac_sh'high) & smaller_frac_sh);
   -- Normalization of fraction
   count_type <= add_frac(14);
   add_frac_shift <= add_frac(13 downto 0) & (smaller_frac(0) AND offset(0));
   FractionNormalizer: Normalizer_ZO_15_15_15_F0_uid14
      port map ( OZb => count_type,
                 X => add_frac_shift,
                 Count => count,
                 R => norm_frac_tmp);
   -- Correct final exponent
   add_sf <= (larger_sf(6) & larger_sf) - ("0000" & count) + 1;
--------------------------- Data Rounding & Encoding ---------------------------
   is_not_zero <= '0' when (count = "1111") else '1';
   is_nar <= X_nar OR Y_nar;
   XY_nzn <= is_not_zero AND NOT(is_nar);
   sign <= is_nar OR (is_not_zero AND add_frac(14));
   norm_frac <= norm_frac_tmp(13 downto 3);
   PositEncoder: PositHUBFastEncoder_16_2_F0_uid16
      port map ( Frac => norm_frac,
                 NZN => XY_nzn,
                 SF => add_sf,
                 Sign => sign,
                 R => R);
---------------------------- End of vhdl generation ----------------------------
end architecture;

