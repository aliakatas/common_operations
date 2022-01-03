#include <math.h>
#include <algorithm>

#define DEBRIS_PASTURE_ARABLE		1
#define DEBRIS_WOODLAND				2
#define DEBRIS_URBAN				3

namespace common_operations {

template <typename T>
int are_equal(const T a, const T b, const T tolerance) {
	return abs(a - b) < tolerance;
}

template <typename T>
void change_unit_system(T* val, const T conversion_factor) {
	*val *= conversion_factor;
}

template <typename T>
void change_unit_system_safe(T* val, const T conversion_factor,
		const T nodata, const T tolerance) {
	if (are_equal(*val, nodata, tolerance))
		return;

	change_unit_system(val, conversion_factor);
}

template <typename T>
T deg_to_rad(const T deg) {
	return deg * M_PI / static_cast<T>(180.);
}

template <typename T>
T deg_to_rad_safe(const T deg, const T nodata, const T tolerance) {
	if (are_equal(deg, nodata, tolerance))
		return nodata;

	return deg_to_rad(deg);
}

template <typename T>
T rad_to_deg(const T rad) {
	return rad * static_cast<T>(180.) / M_PI;
}

template <typename T>
T rad_to_deg_safe(const T rad, const T nodata, const T tolerance) {
	if (are_equal(rad, nodata, tolerance))
		return nodata;

	return rad_to_deg(rad);
}

template <typename T>
T calculate_magnitude(const T x, const T y) {
	return sqrt(x * x + y * y);
}

template <typename T>
T calculate_magnitude_safe(const T x, const T y,
		const T nodata, const T tolerance) {
	if (are_equal(x, nodata, tolerance) ||
			are_equal(y, nodata, tolerance))
		return nodata;

	return calculate_magnitude(x, y);
}

template <typename T>
T calculate_direction(const T x, const T y) {
	return atan2(y, x);
}

template <typename T>
T calculate_direction_safe(const T x, const T y,
		const T nodata, const T tolerance) {
	if (are_equal(x, nodata, tolerance) ||
			are_equal(y, nodata, tolerance))
		return nodata;

	return calculate_direction(x, y);
}

template <typename T>
void translate_point(T* point, const T* translation) {
	point[0] += translation[0];
	point[1] += translation[1];
}

template <typename T>
void translate_point_safe(T* point, const T* translation,
		const T nodata, const T tolerance) {
	if (are_equal(point[0], nodata, tolerance) ||
		are_equal(point[1], nodata, tolerance))
			return;

	translate_point(point, translation);
}

template <typename T>
void set_rotation_matrix(T* rotation_matrix, const T angle_rad) {
	rotation_matrix[0] = cos(angle_rad);
	rotation_matrix[1] = -sin(angle_rad);
	rotation_matrix[2] = sin(angle_rad);
	rotation_matrix[3] = cos(angle_rad);
}

template <typename T>
void rotate_point(T* point, const T* rotation_matrix) {
	T p0 = point[0];
	T p1 = point[1];

	point[0] = rotation_matrix[0] * p0 + rotation_matrix[1] * p1;
	point[1] = rotation_matrix[2] * p0 + rotation_matrix[3] * p1;
}

template <typename T>
void rotate_point_safe(T* point, const T* rotation_matrix,
		const T nodata, const T tolerance) {
	if (are_equal(point[0], nodata, tolerance) ||
			are_equal(point[1], nodata, tolerance))
		return;

	rotate_point(point, rotation_matrix);
}

template <typename T>
void rotate_point_about(T* point, const T* origin, const T* rotation_matrix) {
	translate_point(point, {-origin[0], -origin[1]});
	rotate_point(point, rotation_matrix);
	translate_point(point, origin);
}

template <typename T>
void rotate_point_about_safe(T* point, const T* origin, const T* rotation_matrix,
		const T nodata, const T tolerance) {
	if (are_equal(point[0], nodata, tolerance) ||
			are_equal(point[1], nodata, tolerance))
		return;
	rotate_point_about(point, origin, rotation_matrix);
}

template <typename T>
void calculate_unit_vector(T* unit_vec, const T* original_vec) {
	T mag = calculate_magnitude(original_vec[0], original_vec[1]);
	if (mag < static_cast<T>(1.e-7)) {
		unit_vec[0] = static_cast<T>(0.);
		unit_vec[1] = static_cast<T>(0.);
	}
	else {
		unit_vec[0] = original_vec[0] / mag;
		unit_vec[1] = original_vec[1] / mag;
	}
}

template <typename T>
T dot_product(const T* vector_a, const T* vector_b) {
   return vector_a[0] * vector_b[0] + vector_a[1] * vector_b[1];
}

template <typename T>
T cross_product(const T* vector_a, const T* vector_b) {
   return vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];
}

template <typename T>
T calculate_parallel_component(const T* vec,
		const T* ref_line_start, const T* ref_line_end) {
	T line_vec[2] = { ref_line_end[0] - ref_line_start[0],
			ref_line_end[1] - ref_line_start[1] };
	T unit_line_vec[2];
	calculate_unit_vector(unit_line_vec, line_vec);

	return dot_product(vec, unit_line_vec);
}

template <typename T>
T calculate_parallel_component_safe(const T* vec,
		const T* ref_line_start, const T* ref_line_end,
		const T nodata, const T tolerance) {
	if (are_equal(vec[0], nodata, tolerance) ||
			are_equal(vec[1], nodata, tolerance)) {
		return nodata;
	}
	return calculate_parallel_component(vec, ref_line_start, ref_line_end);
}

template <typename T>
T calculate_normal_component(const T* vec,
		const T* ref_line_start, const T* ref_line_end) {
	T line_vec[2] = { ref_line_end[0] - ref_line_start[0],
				ref_line_end[1] - ref_line_start[1] };
	T unit_line_vec[2];
	calculate_unit_vector(unit_line_vec, line_vec);
	return cross_product(unit_line_vec, vec);
}

template <typename T>
T calculate_normal_component_safe(const T* vec,
		const T* ref_line_start, const T* ref_line_end,
		const T nodata, const T tolerance) {
	if (are_equal(vec[0], nodata, tolerance) ||
			are_equal(vec[1], nodata, tolerance)) {
		return nodata;
	}
	return calculate_normal_component(vec, ref_line_start, ref_line_end);
}

template <typename T>
T calculate_depth(const T bed_elevation, const T water_elevation,
		const int bed_elevation_conv = 1) {
	return bed_elevation + bed_elevation_conv * water_elevation;
}

template <typename T>
T calculate_depth_safe(const T bed_elevation, const T water_elevation,
		const T nodata, const T tolerance,
		const int bed_elevation_conv = 1) {
	if (are_equal(bed_elevation, nodata, tolerance) ||
			are_equal(water_elevation, nodata, tolerance))
		return nodata;
	return calculate_depth(bed_elevation,
			water_elevation, bed_elevation_conv);
}

template <typename T>
T calculate_velocity(const T discharge, const T depth,
		const T min_depth) {
	return discharge / std::max(depth, min_depth);
}

template <typename T>
T calculate_velocity_safe(const T discharge, const T depth,
		const T min_depth,
		const T nodata, const T tolerance) {
	if (are_equal(discharge, nodata, tolerance) ||
			are_equal(depth, nodata, tolerance))
		return nodata;
	return calculate_velocity(discharge, depth, min_depth);
}

template <typename T>
T calculate_froude_number(const T velocity, const T depth,
		const T min_depth, const T gravity) {
	return velocity / sqrt(gravity * std::max(depth, min_depth));
}

template <typename T>
T calculate_froude_number_safe(const T velocity, const T depth,
		const T min_depth, const T gravity,
		const T nodata, const T tolerance) {
	if (are_equal(velocity, nodata, tolerance) ||
			are_equal(depth, nodata, tolerance))
		return nodata;
	return calculate_froude_number(velocity, depth, min_depth, gravity);
}

template <typename T>
T calculate_kinetic_energy(const T velocity) {
	return static_cast<T>(0.5) * velocity * velocity;
}

template <typename T>
T calculate_kinetic_energy_safe(const T velocity,
		const T nodata, const T tolerance) {
	if (are_equal(velocity, nodata, tolerance))
		return nodata;
	return calculate_kinetic_energy(velocity);
}

template <typename T>
T calculate_potential_energy(const T water_elevation, const T gravity) {
	return water_elevation * gravity;
}

template <typename T>
T calculate_potential_energy_safe(const T water_elevation, const T gravity,
		const T nodata, const T tolerance) {
	if (are_equal(water_elevation, nodata, tolerance))
		return nodata;
	return calculate_potential_energy(water_elevation, gravity);
}

template <typename T>
T calculate_courant_number(const T velocity,
		const T time_resolution, const T space_resolution) {
	return velocity * time_resolution / space_resolution;
}

template <typename T>
T calculate_courant_number(const T velocity_x, const T velocity_y,
		const T time_resolution,
		const T space_x_resolution, const T space_y_resolution) {
	return calculate_courant_number(velocity_x, time_resolution, space_x_resolution) +
			calculate_courant_number(velocity_y, time_resolution, space_y_resolution);
}

template <typename T>
T calculate_courant_number_safe(const T velocity,
		const T time_resolution, const T space_resolution,
		const T nodata, const T tolerance) {
	if (are_equal(velocity, nodata, tolerance))
		return nodata;
	return calculate_courant_number(velocity, time_resolution, space_resolution);
}

template <typename T>
T calculate_courant_number_safe(const T velocity_x, const T velocity_y,
		const T time_resolution, const T space_x_resolution, const T space_y_resolution,
		const T nodata, const T tolerance) {
	if (are_equal(velocity_x, nodata, tolerance) ||
			are_equal(velocity_y, nodata, tolerance))
		return nodata;
	return calculate_courant_number(velocity_x, time_resolution, space_x_resolution) +
				calculate_courant_number(velocity_y, time_resolution, space_y_resolution);
}

// based on http://randd.defra.gov.uk/Document.aspx?Document=FD2321_7400_PR.pdf
template <typename T>
T calculate_flood_hazard_rating(const T depth, const T velocity,
		const T const_coef, const T debris_coef) {
	return depth + (velocity + const_coef) + debris_coef;
}

template <typename T>
T calculate_flood_hazard_rating_safe(const T depth, const T velocity,
		const T const_coef, const T debris_coef,
		const T nodata, const T tolerance) {
	if (are_equal(velocity, nodata, tolerance) ||
			are_equal(depth, nodata, tolerance))
		return nodata;
	return calculate_flood_hazard_rating(depth, velocity, const_coef, debris_coef);
}

template <typename T>
T calculate_debris_coefficient(const T depth, const T velocity,
		const int debris_characterisation) {
	// High depth or high velocity
	if (depth > static_cast<T>(0.75) ||
			velocity > static_cast<T>(2.)) {
		switch (debris_characterisation) {
		case DEBRIS_PASTURE_ARABLE:
			return static_cast<T>(0.5);
		case DEBRIS_WOODLAND:
		case DEBRIS_URBAN:
			return static_cast<T>(1.0);
		default:
			return static_cast<T>(0.);
		}
	}

	// Low depth
	if ((depth >= static_cast<T>(0.)) &&
			(depth <= static_cast<T>(0.25)))
		return static_cast<T>(0.);

	// Mid depth
	if ((depth > static_cast<T>(0.25)) &&
			(depth <= static_cast<T>(0.75))) {
		switch (debris_characterisation) {
		case DEBRIS_PASTURE_ARABLE:
			return static_cast<T>(0.);
		case DEBRIS_WOODLAND:
			return static_cast<T>(0.5);
		case DEBRIS_URBAN:
			return static_cast<T>(1.0);
		default:
			return static_cast<T>(0.);
		}
	}

	return static_cast<T>(0.);
}
}

