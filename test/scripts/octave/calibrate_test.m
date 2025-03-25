# Generate Sensor Characterization
sensor_pixel_sensitivity_table = generate_pixel_sensitivity_table(64,64);

# Generate a Circle Reference Image
image_circle = generate_image_circle(64,64,32,32,10,0.15,0.5);
image_circle_acquired = apply_sensor_model(image_circle, sensor_pixel_sensitivity_table, 0.0, 0.0);

# figure; subplot(2,1,1); imshow(image_circle); subplot(2,1,2); imshow(image_circle_acquired);
# figure; subplot(2,1,1); imagesc(image_circle); subplot(2,1,2); imagesc(image_circle_acquired);

# Generate a Minimum Offset as A Uniform Temperature Reference at Minimum Temperature
image_min = generate_image_uniform(64,64, 0.1);
image_min_acquired = apply_sensor_model(image_min, sensor_pixel_sensitivity_table, 0.0, 1e-9);

# figure; subplot(2,1,1); imshow(image_min); subplot(2,1,2); imshow(image_min_acquired);
# figure; subplot(2,1,1); imagesc(image_min); subplot(2,1,2); imagesc(image_min_acquired);

# Apply Sensor Model to Generate Circle image
image_max = generate_image_uniform(64,64, 0.9);
image_max_acquired = apply_sensor_model(image_max, sensor_pixel_sensitivity_table, 0.0, 0.0);

# figure; subplot(2,1,1); imshow(image_max); subplot(2,1,2); imshow(image_max_acquired);
# figure; subplot(2,1,1); imagesc(image_max); subplot(2,1,2); imagesc(image_max_acquired);

figure;

subplot(3,3,1); imagesc(image_circle);
subplot(3,3,2); imagesc(image_min);
subplot(3,3,3); imagesc(image_max);
subplot(3,3,4); imagesc(image_circle_acquired);
subplot(3,3,5); imagesc(image_min_acquired);
subplot(3,3,6); imagesc(image_max_acquired);

subplot(3,3,7); hist(image_circle_acquired);
subplot(3,3,8); hist(image_min_acquired);
subplot(3,3,9); hist(image_max_acquired);


[scale, offset] = calibrate(image_min_acquired, image_max_acquired);

image_circle_adjusted = image_circle_acquired .* scale + offset;

figure;
subplot(1,2,1); imagesc(image_circle_acquired);
subplot(1,2,2); imagesc(image_circle_adjusted);
