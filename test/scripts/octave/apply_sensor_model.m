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

function retval = apply_sensor_model(image, pixel_sensitivity_table, mu = 0, sigma = 1e-6, integration_samples = 1000)

      [width, height] = size(image);

      # for now while I don't have a complete version of octave, write with
      # the simple rand function
      retval = image .* pixel_sensitivity_table; + normrnd(mu, sigma, width, height);

      # for now while I don't have a complete version of octave, write with
      # the simple rand function
      for i = 1:integration_samples
        retval += image .* pixel_sensitivity_table; + normrnd(mu, sigma, width, height);
      end

endfunction
