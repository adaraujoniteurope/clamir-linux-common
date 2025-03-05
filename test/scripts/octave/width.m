function w = width(m00, m01, m10, m11, m02, m20)

    if m00 == 0
        m00 = 1
    end

    [x,y] = centroid(m00,m10, m01);

    u20 = (m20 / m00) - (x * y);
    u11 = (m11 / m00) - (x * y);
    u02 = (m02 / m00) - (x * y);

    u22 = (u20 - u02);

    w = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02)))));

end
