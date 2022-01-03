
#define BOOST_TEST_MODULE Common Operations Testcases
#include <boost/test/unit_test.hpp>

#include "common_operations.h"

BOOST_AUTO_TEST_SUITE(are_equal)

BOOST_AUTO_TEST_CASE(test1) {
    BOOST_CHECK_EQUAL(
    		common_operations::are_equal(9999., 9999.001, 0.01) , 1);
}

BOOST_AUTO_TEST_CASE(test2) {
	BOOST_CHECK_EQUAL(
	        common_operations::are_equal(9999., 9999.001, 0.001) , 0);
}

BOOST_AUTO_TEST_CASE(test3) {
    BOOST_CHECK_EQUAL(
        	common_operations::are_equal(-9.f, -9.001f, 0.01f) , 1);
}

BOOST_AUTO_TEST_CASE(test4) {
	BOOST_CHECK_EQUAL(
	        common_operations::are_equal(9.f, 9.001f, 0.001f) , 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(change_units)

double val1 = 1.;
BOOST_AUTO_TEST_CASE(test1) {
	common_operations::change_unit_system_safe(&val1, 0.33, 9999., 0.001);
    BOOST_CHECK_CLOSE(val1, 0.33, 0.0001);
}

double val2 = 999.;
BOOST_AUTO_TEST_CASE(test2) {
	common_operations::change_unit_system_safe(&val2, 2., 999., 0.001);
    BOOST_CHECK_CLOSE(val2, 999., 0.0001);
}

float val3 = 8.f;
BOOST_AUTO_TEST_CASE(test3) {
	common_operations::change_unit_system_safe(&val3, 0.4f, 999.f, 0.001f);
    BOOST_CHECK_CLOSE(val3, 3.2f, 0.0001f);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(angles_conversion)

BOOST_AUTO_TEST_CASE(test1) {
    BOOST_CHECK_CLOSE(common_operations::deg_to_rad_safe(45., 999., 0.01),
    		M_PI / 4., 0.0001);
}

BOOST_AUTO_TEST_CASE(test2) {
    BOOST_CHECK_CLOSE(common_operations::deg_to_rad_safe(-999., -999., 0.01),
    		-999., 0.0001);
}

BOOST_AUTO_TEST_CASE(test3) {
    BOOST_CHECK_CLOSE(common_operations::rad_to_deg_safe(M_PI / 3., 999., 0.01),
    		60., 0.0001);
}

BOOST_AUTO_TEST_CASE(test4) {
    BOOST_CHECK_CLOSE(common_operations::rad_to_deg_safe(999., 999., 0.01),
    		999., 0.0001);
}

BOOST_AUTO_TEST_CASE(test5) {
    BOOST_CHECK_CLOSE(common_operations::rad_to_deg_safe(0., 999., 0.01),
    		0., 0.0001);
}

BOOST_AUTO_TEST_CASE(test6) {
    BOOST_CHECK_CLOSE(common_operations::deg_to_rad_safe(0., 999., 0.01),
    		0., 0.0001);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(vector_operations)

BOOST_AUTO_TEST_CASE(test1) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_magnitude_safe(1., 1., 999., 0.01),
    		sqrt(2.), 0.0001);
}

BOOST_AUTO_TEST_CASE(test2) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_magnitude_safe(0., 1., 999., 0.01),
    		1., 0.0001);
}

BOOST_AUTO_TEST_CASE(test3) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_magnitude_safe(999., 1., 999., 0.01),
    		999., 0.0001);
}

BOOST_AUTO_TEST_CASE(test4) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_magnitude_safe(999., 999., 999., 0.01),
    		999., 0.0001);
}

BOOST_AUTO_TEST_CASE(test5) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_magnitude_safe(3., 4., 999., 0.01),
    		5., 0.0001);
}

BOOST_AUTO_TEST_CASE(test6) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_direction_safe(3., 3., 999., 0.01),
    		M_PI / 4., 0.0001);
}

BOOST_AUTO_TEST_CASE(test7) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_direction_safe(-999., 3., -999., 0.01),
    		-999., 0.0001);
}

BOOST_AUTO_TEST_CASE(test8) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_direction_safe(1., sqrt(3.), -999., 0.01),
    		M_PI / 3., 0.0001);
}

BOOST_AUTO_TEST_CASE(test9) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_direction_safe(0., 0., -999., 0.01),
    		0., 0.0001);
}

BOOST_AUTO_TEST_CASE(test10) {
    BOOST_CHECK_CLOSE(
    		common_operations::calculate_direction_safe(-1., 0., -999., 0.01),
    		M_PI, 0.0001);
}

BOOST_AUTO_TEST_SUITE_END()
