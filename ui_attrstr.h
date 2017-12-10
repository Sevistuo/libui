// uiAttribute stores information about an attribute in a
// uiAttributedString.
//
// You do not create uiAttributes directly; instead, you create a
// uiAttribute of a given type using the specialized constructor
// functions. For every Unicode codepoint in the uiAttributedString,
// at most one value of each attribute type can be applied.
//
// uiAttributes are immutable and the uiAttributedString takes
// ownership of the uiAttribute object once assigned, copying its
// contents as necessary.
typedef struct uiAttribute uiAttribute;

// uiFreeAttribute() frees a uiAttribute. You generally do not need to
// call this yourself, as uiAttributedString does this for you. In fact,
// it is an error to call this function on a uiAttribute that has been
// given to a uiAttributedString. You can call this, however, if you
// created a uiAttribute that you aren't going to use later.
_UI_EXTERN void uiFreeAttribute(uiAttribute *a);

// uiAttributeType holds the possible uiAttribute types that may be
// returned by uiAttributeGetType(). Refer to the documentation for
// each type's constructor function for details on each type.
_UI_ENUM(uiAttributeType) {
	uiAttributeTypeFamily,
	uiAttributeTypeSize,
	uiAttributeTypeWeight,
	uiAttributeTypeItalic,
	uiAttributeTypeStretch,
	uiAttributeTypeColor,
	uiAttributeTypeBackground,
	uiAttributeTypeUnderline,
	uiAttributeTypeUnderlineColor,
	uiAttributeTypeFeatures,
};

// uiAttributeGetType() returns the type of a.
// TODO I don't like this name
_UI_EXTERN uiAttributeType uiAttributeGetType(const uiAttribute *a);

// uiNewFamilyAttribute() creates a new uiAttribute that changes the
// font family of the text it is applied to. Font family names are
// case-insensitive.
_UI_EXTERN uiAttribute *uiNewFamilyAttribute(const char *family);

// uiAttributeFamily() returns the font family stored in a. The
// returned string is owned by a. It is an error to call this on a
// uiAttribute that does not hold a font family.
_UI_EXTERN const char *uiAttributeFamily(const uiAttribute *a);

// uiNewSizeAttribute() creates a new uiAttribute that changes the
// size of the text it is applied to, in typographical points.
_UI_EXTERN uiAttribute *uiNewFamilyAttribute(double size);

// uiAttributeSize() returns the font size stored in a. It is an error to
// call this on a uiAttribute that does not hold a font size.
_UI_EXTERN double uiAttributeSize(const uiAttribute *a);

	// uiAttributeWeight changes the weight of the text it is applied
	// to. Use the Weight field of uiAttributeSpec.
	uiAttributeTypeWeight,
	// uiAttributeItalic changes the italicness of the text it is applied
	// to. Use the Italic field of uiAttributeSpec.
	uiAttributeTypeItalic,
	// uiAttributeStretch changes the stretch of the text it is applied
	// to. Use the Stretch field of uiAttributeSpec.
	uiAttributeTypeStretch,
	// uiAttributeColor changes the color of the text it is applied to.
	// Use the R, G, B, and A fields of uiAttributeSpec.
	uiAttributeTypeColor,
	// uiAttributeBackground changes the color of the text it is
	// applied to. Use the R, G, B, and A fields of uiAttributeSpec.
	uiAttributeTypeBackground,

	// uiAttributeUnderline changes the underline style of the text
	// it is applied to. Use the UnderlineStyle field of
	// uiAttributeSpec.
	uiAttributeTypeUnderline,
	// uiAttributeUnderlineColor changes the color of any underline
	// on the text it is applied to, regardless of the style. Use the
	// UnderlineColor field of uiAttributeSpec, and also the R, G, B,
	// and A fields if specifying uiDrawUnderlineColorCustom.
	// 
	// If an underline style is applied but no underline color is
	// specified, the text color is used instead. If an underline color
	// is specified without an underline style, the underline color
	// attribute is ignored, but not elided.
	uiAttributeTypeUnderlineColor,

	// uiAttributeFeatures changes the OpenType features of the
	// text it is applied to. Use the Features field of uiAttributeSpec.
	uiAttributeTypeFeatures,
};

_UI_ENUM(uiUnderlineStyle) {
	uiUnderlineStyleNone,
	uiUnderlineStyleSingle,
	uiUnderlineStyleDouble,
	uiUnderlineStyleSuggestion,		// wavy or dotted underlines used for spelling/grammar checkers
};

_UI_ENUM(uiUnderlineColor) {
	uiUnderlineColorCustom,
	uiUnderlineColorSpelling,
	uiUnderlineColorGrammar,
	uiUnderlineColorAuxiliary,		// for instance, the color used by smart replacements on OS X
};

// uiOpenTypeFeatures represents a set of OpenType feature
// tag-value pairs, for applying OpenType features to text.
// OpenType feature tags are four-character codes defined by
// OpenType that cover things from design features like small
// caps and swashes to language-specific glyph shapes and
// beyond. Each tag may only appear once in any given
// uiOpenTypeFeatures instance. Each value is a 32-bit integer,
// often used as a Boolean flag, but sometimes as an index to choose
// a glyph shape to use.
// 
// If a font does not support a certain feature, that feature will be
// ignored. (TODO verify this on all OSs)
// 
// See the OpenType specification at
// https://www.microsoft.com/typography/otspec/featuretags.htm
// for the complete list of available features, information on specific
// features, and how to use them.
// TODO invalid features
typedef struct uiOpenTypeFeatures uiOpenTypeFeatures;

// uiOpenTypeFeaturesForEachFunc is the type of the function
// invoked by uiOpenTypeFeaturesForEach() for every OpenType
// feature in otf. Refer to that function's documentation for more
// details.
typedef uiForEach (*uiOpenTypeFeaturesForEachFunc)(const uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t value, void *data);

// @role uiOpenTypeFeatures constructor
// uiNewOpenTypeFeatures() returns a new uiOpenTypeFeatures
// instance, with no tags yet added.
_UI_EXTERN uiOpenTypeFeatures *uiNewOpenTypeFeatures(void);

// @role uiOpenTypeFeatures destructor
// uiFreeOpenTypeFeatures() frees otf.
_UI_EXTERN void uiFreeOpenTypeFeatures(uiOpenTypeFeatures *otf);

// uiOpenTypeFeaturesClone() makes a copy of otf and returns it.
// Changing one will not affect the other.
_UI_EXTERN uiOpenTypeFeatures *uiOpenTypeFeaturesClone(const uiOpenTypeFeatures *otf);

// uiOpenTypeFeaturesAdd() adds the given feature tag and value
// to otf. The feature tag is specified by a, b, c, and d. If there is
// already a value associated with the specified tag in otf, the old
// value is removed.
_UI_EXTERN void uiOpenTypeFeaturesAdd(uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t value);

// uiOpenTypeFeaturesRemove() removes the given feature tag
// and value from otf. If the tag is not present in otf,
// uiOpenTypeFeaturesRemove() does nothing.
_UI_EXTERN void uiOpenTypeFeaturesRemove(uiOpenTypeFeatures *otf, char a, char b, char c, char d);

// uiOpenTypeFeaturesGet() determines whether the given feature
// tag is present in otf. If it is, *value is set to the tag's value and
// nonzero is returned. Otherwise, zero is returned.
// 
// Note that if uiOpenTypeFeaturesGet() returns zero, value isn't
// changed. This is important: if a feature is not present in a
// uiOpenTypeFeatures, the feature is NOT treated as if its
// value was zero anyway. Script-specific font shaping rules and
// font-specific feature settings may use a different default value
// for a feature. You should likewise not treat a missing feature as
// having a value of zero either. Instead, a missing feature should
// be treated as having some unspecified default value.
_UI_EXTERN int uiOpenTypeFeaturesGet(const uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t *value);

// uiOpenTypeFeaturesForEach() executes f for every tag-value
// pair in otf. The enumeration order is unspecified. You cannot
// modify otf while uiOpenTypeFeaturesForEach() is running.
_UI_EXTERN void uiOpenTypeFeaturesForEach(const uiOpenTypeFeatures *otf, uiOpenTypeFeaturesForEachFunc f, void *data);

// uiOpenTypeFeaturesEqual() returns nonzero if a is equal to b.
// a is defined as equal to b if and only if both have exactly the same
// tags with exactly the same values, or if both are NULL.
_UI_EXTERN int uiOpenTypeFeaturesEqual(const uiOpenTypeFeatures *a, const uiOpenTypeFeatures *b);

typedef struct uiAttributeSpec uiAttributeSpec;

// TODO note that pointers are copied
// TODO add a function to uiAttributedString to get an attribute's value at a specific index or in a specific range, so we can edit
// (TODO related to above: what about memory consumption during a read-modify-write cycle due to copying?)
struct uiAttributeSpec {
	uiAttribute Type;
	const char *Family;
	double Size;
	uiDrawTextWeight Weight;
	uiDrawTextItalic Italic;
	uiDrawTextStretch Stretch;
	double R;
	double G;
	double B;
	double A;
	uiDrawUnderlineStyle UnderlineStyle;
	uiDrawUnderlineColor UnderlineColor;
	const uiOpenTypeFeatures *Features;
};

typedef struct uiDrawFontDescriptor uiDrawFontDescriptor;

_UI_ENUM(uiDrawTextWeight) {
	uiDrawTextWeightMinimum = 0,
	uiDrawTextWeightThin = 100,
	uiDrawTextWeightUltraLight = 200,
	uiDrawTextWeightLight = 300,
	uiDrawTextWeightBook = 350,
	uiDrawTextWeightNormal = 400,
	uiDrawTextWeightMedium = 500,
	uiDrawTextWeightSemiBold = 600,
	uiDrawTextWeightBold = 700,
	uiDrawTextWeightUltraBold = 800,
	uiDrawTextWeightHeavy = 900,
	uiDrawTextWeightUltraHeavy = 950,
	uiDrawTextWeightMaximum = 1000,
};

_UI_ENUM(uiDrawTextItalic) {
	uiDrawTextItalicNormal,
	uiDrawTextItalicOblique,
	uiDrawTextItalicItalic,
};

_UI_ENUM(uiDrawTextStretch) {
	uiDrawTextStretchUltraCondensed,
	uiDrawTextStretchExtraCondensed,
	uiDrawTextStretchCondensed,
	uiDrawTextStretchSemiCondensed,
	uiDrawTextStretchNormal,
	uiDrawTextStretchSemiExpanded,
	uiDrawTextStretchExpanded,
	uiDrawTextStretchExtraExpanded,
	uiDrawTextStretchUltraExpanded,
};

struct uiDrawFontDescriptor {
	char *Family;
	double Size;
	uiDrawTextWeight Weight;
	uiDrawTextItalic Italic;
	uiDrawTextStretch Stretch;
};

typedef struct uiDrawTextLayout uiDrawTextLayout;
typedef struct uiDrawTextLayoutParams uiDrawTextLayoutParams;
typedef struct uiDrawTextLayoutLineMetrics uiDrawTextLayoutLineMetrics;

_UI_ENUM(uiDrawTextAlign) {
	uiDrawTextAlignLeft,
	uiDrawTextAlignCenter,
	uiDrawTextAlignRight,
};

struct uiDrawTextLayoutParams {
	uiAttributedString *String;
	uiDrawFontDescriptor *DefaultFont;
	double Width;
	uiDrawTextAlign Align;
};

// Height will equal ParagraphSpacingBefore + LineHeightSpace + Ascent + Descent + Leading + LineSpacing + ParagraphSpacing.
// The above values are listed in vertical order, from top to bottom.
// Ascent + Descent + Leading will give you the typographic bounds
// of the text. BaselineY is the boundary between Ascent and Descent.
// X, Y, and BaselineY are all in the layout's coordinate system, so the
// start point of the baseline will be at (X, BaselineY). All values are
// nonnegative.
struct uiDrawTextLayoutLineMetrics {
	// This describes the overall bounding box of the line.
	double X;
	double Y;
	double Width;
	double Height;

	// This describes the typographic bounds of the line.
	double BaselineY;
	double Ascent;
	double Descent;
	double Leading;

	// This describes any additional whitespace.
	// TODO come up with better names for these.
	double ParagraphSpacingBefore;
	double LineHeightSpace;
	double LineSpacing;
	double ParagraphSpacing;

	// TODO trailing whitespace?
};

// TODO
// - allow creating a layout out of a substring
// - allow marking compositon strings
// - allow marking selections, even after creation
// - add the following functions:
// 	- uiDrawTextLayoutHeightForWidth() (returns the height that a layout would need to be to display the entire string at a given width)
// 	- uiDrawTextLayoutRangeForSize() (returns what substring would fit in a given size)
// 	- uiDrawTextLayoutNewWithHeight() (limits amount of string used by the height)
// - some function to fix up a range (for text editing)
_UI_EXTERN uiDrawTextLayout *uiDrawNewTextLayout(uiDrawTextLayoutParams *params);
_UI_EXTERN void uiDrawFreeTextLayout(uiDrawTextLayout *tl);
_UI_EXTERN void uiDrawText(uiDrawContext *c, uiDrawTextLayout *tl, double x, double y);
_UI_EXTERN void uiDrawTextLayoutExtents(uiDrawTextLayout *tl, double *width, double *height);
_UI_EXTERN int uiDrawTextLayoutNumLines(uiDrawTextLayout *tl);
_UI_EXTERN void uiDrawTextLayoutLineByteRange(uiDrawTextLayout *tl, int line, size_t *start, size_t *end);
_UI_EXTERN void uiDrawTextLayoutLineGetMetrics(uiDrawTextLayout *tl, int line, uiDrawTextLayoutLineMetrics *m);
// TODO number of lines visible for clipping rect, range visible for clipping rect?

// TODO rewrite all this documentation

// uiDrawTextLayoutHitTest() returns the byte offset and line closest
// to the given point. The point is relative to the top-left of the layout.
// If the point is outside the layout itself, the closest point is chosen;
// this allows the function to be used for cursor positioning with the
// mouse. Do keep the returned line in mind if used in this way; the
// user might click on the end of a line, at which point the cursor
// might be at the trailing edge of the last grapheme on the line
// (subject to the operating system's APIs).
_UI_EXTERN void uiDrawTextLayoutHitTest(uiDrawTextLayout *tl, double x, double y, size_t *pos, int *line);

// uiDrawTextLayoutByteLocationInLine() returns the point offset
// into the given line that the given byte position stands. This is
// relative to the line's X position (as returned by
// uiDrawTextLayoutLineGetMetrics()), which in turn is relative to
// the top-left of the layout. This function can be used for cursor
// positioning: if start and end are the start and end of the line
// (as returned by uiDrawTextLayoutLineByteRange()), you will get
// the correct offset, even if pos is at the end of the line. If pos is not
// in the range [start, end], a negative value will be returned,
// indicating you need to move the cursor to another line.
// TODO make sure this works right for right-aligned and center-aligned lines and justified lines and RTL text
_UI_EXTERN double uiDrawTextLayoutByteLocationInLine(uiDrawTextLayout *tl, size_t pos, int line);

_UI_EXTERN void uiDrawCaret(uiDrawContext *c, double x, double y, uiDrawTextLayout *layout, size_t pos, int *line);
// TODO allow blinking
// TODO allow secondary carets

typedef struct uiFontButton uiFontButton;
#define uiFontButton(this) ((uiFontButton *) (this))
// TODO have a function that sets an entire font descriptor to a range in a uiAttributedString at once, for SetFont?
_UI_EXTERN void uiFontButtonFont(uiFontButton *b, uiDrawFontDescriptor *desc);
// TOOD SetFont, mechanics
_UI_EXTERN void uiFontButtonOnChanged(uiFontButton *b, void (*f)(uiFontButton *, void *), void *data);
_UI_EXTERN uiFontButton *uiNewFontButton(void);
