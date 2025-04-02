## Copyright (C) 2025
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {@var{retval} =} generate_default_scale (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author:  <arthur@arthur-notebook-victus>
## Created: 2025-04-01

function retval = generate_default_scale (file, width = 64, height = 64)
  fid = fopen(file, "w");
  scale = 2.^14*ones(width,height);
  fwrite(fid, scale, "int32");
  fclose(fid);
  retval = 0
endfunction
