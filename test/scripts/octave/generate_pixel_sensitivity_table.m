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
## @deftypefn {} {@var{retval} =} apply_sensor_model (@var{image}, @var{pixel_sensitivity_table}, @var{mu}, @var{sigma}, @var{integration_samples})
##
## @seealso{}
## @end deftypefn

## Author:  <arthur@arthur-notebook-victus>
## Created: 2025-03-25

function retval = generate_pixel_sensitivity_table(width = 64, height = 64, mu = 1.0e-3, sigma = 0.1e-3)
  retval = normrnd(mu, sigma, width, height);
endfunction
