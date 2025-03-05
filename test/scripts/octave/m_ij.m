function _m_ij = m_ij(arr, i, j)

    _m_ij = 0;

    for c = 1:length(arr)
      for r = 1:length(arr(c,:))
        _m_ij = _m_ij + arr(c,r) * c.^i * r.^j;
      endfor
    endfor

end
