image = generate_image(64,64, 15, 15, 5);

imagesc(image);

M00 = m_ij(image,0,0);
M01 = m_ij(image,0,1);
M10 = m_ij(image,1,0);
M11 = m_ij(image,1,1);
M02 = m_ij(image,0,2);
M20 = m_ij(image,2,0);

[x, y] = centroid(M00, M10, M01);

width_ = width(M00, M01, M10, M11, M02, M20);
