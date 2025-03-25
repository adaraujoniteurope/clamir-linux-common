function image = generate_image_circle(width, height, x = width/2, y = height/2, radius = 10, value = 0.5, offset = 0.25)

    image = zeros(width, height);

    for c = 1:width
        for r = 1:height
            _radius = sqrt((x - c).^2+(y-r).^2);
            if (_radius < radius)
                image(c,r) = value - offset;
            end
        end
    end

    image += offset;

end
