namespace math {
	template < typename _type >
	V_INLINE _type clamp( _type val, _type min, _type max ) V_NOEXCEPT {
		return val >= min ? ( val <= max ? val : max ) : min;
	}

	template < typename _type >
	V_INLINE _type clamp( _type val, _type max ) V_NOEXCEPT {
		return val <= max ? val : max;
	}
}