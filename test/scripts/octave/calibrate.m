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
## @deftypefn {} {@var{retval} =} auto_calibrate (@var{input1}, @var{input2})
##
## @seealso{}
## @end deftypefn

## Author:  <arthur@arthur-notebook-victus>
## Created: 2025-03-25

function [scale, offset] = auto_offset(image_min, image_max)

  min_avg = mean(mean(image_min));
  max_avg = mean(mean(image_max));

  p_xy_min = mean(mean(image_min));
  p_xy_max = mean(mean(image_max));

  [width, height] = size(image_min);

  scale = zeros(width,height);
  offset = zeros(width,height);

  for i = 1:width
    for j = 1:height

      A = [ image_min(i,j) 1; image_max(i,j) 1 ];
      A_inv = A^(-1);
      Y = [ p_xy_min; p_xy_max ];

      # linear fit of pixel for each pixel
      X = (A_inv * Y);

      scale(i,j) = X(1);
      offset(i,j) = X(2);

    endfor
  endfor

endfunction
