font:

    defb 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   ; U+0020 (space)
    defb 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00   ; U+0021 (!)
    defb 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   ; U+0022 (")
    defb 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00   ; U+0023 (#)
    defb 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00   ; U+0024 ($)
    defb 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00   ; U+0025 (%)
    defb 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00   ; U+0026 (&)
    defb 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00   ; U+0027 (')
    defb 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00   ; U+0028 (()
    defb 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00   ; U+0029 ())
    defb 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00   ; U+002A (*)
    defb 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00   ; U+002B (+)
    defb 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06   ; U+002C (,)
    defb 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00   ; U+002D (-)
    defb 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00   ; U+002E (.)
    defb 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00   ; U+002F (/)
    defb 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00   ; U+0030 (0)
    defb 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00   ; U+0031 (1)
    defb 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00   ; U+0032 (2)
    defb 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00   ; U+0033 (3)
    defb 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00   ; U+0034 (4)
    defb 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00   ; U+0035 (5)
    defb 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00   ; U+0036 (6)
    defb 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00   ; U+0037 (7)
    defb 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00   ; U+0038 (8)
    defb 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00   ; U+0039 (9)
    defb 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00   ; U+003A (:)
    defb 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06   ; U+003B (;)
    defb 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00   ; U+003C (<)
    defb 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00   ; U+003D (=)
    defb 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00   ; U+003E (>)
    defb 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00   ; U+003F (?)
    defb 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00   ; U+0040 (@)
    defb 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00   ; U+0041 (A)
    defb 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00   ; U+0042 (B)
    defb 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00   ; U+0043 (C)
    defb 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00   ; U+0044 (D)
    defb 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00   ; U+0045 (E)
    defb 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00   ; U+0046 (F)
    defb 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00   ; U+0047 (G)
    defb 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00   ; U+0048 (H)
    defb 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00   ; U+0049 (I)
    defb 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00   ; U+004A (J)
    defb 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00   ; U+004B (K)
    defb 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00   ; U+004C (L)
    defb 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00   ; U+004D (M)
    defb 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00   ; U+004E (N)
    defb 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00   ; U+004F (O)
    defb 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00   ; U+0050 (P)
    defb 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00   ; U+0051 (Q)
    defb 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00   ; U+0052 (R)
    defb 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00   ; U+0053 (S)
    defb 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00   ; U+0054 (T)
    defb 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00   ; U+0055 (U)
    defb 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00   ; U+0056 (V)
    defb 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00   ; U+0057 (W)
    defb 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00   ; U+0058 (X)
    defb 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00   ; U+0059 (Y)
    defb 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00   ; U+005A (Z)
    defb 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00   ; U+005B ([)
    defb 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00   ; U+005C (\)
    defb 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00   ; U+005D (])
    defb 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00   ; U+005E (^)
    defb 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF   ; U+005F (_)
    defb 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00   ; U+0060 (`)
    defb 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00   ; U+0061 (a)
    defb 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00   ; U+0062 (b)
    defb 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00   ; U+0063 (c)
    defb 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00   ; U+0064 (d)
    defb 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00   ; U+0065 (e)
    defb 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00   ; U+0066 (f)
    defb 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F   ; U+0067 (g)
    defb 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00   ; U+0068 (h)
    defb 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00   ; U+0069 (i)
    defb 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E   ; U+006A (j)
    defb 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00   ; U+006B (k)
    defb 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00   ; U+006C (l)
    defb 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00   ; U+006D (m)
    defb 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00   ; U+006E (n)
    defb 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00   ; U+006F (o)
    defb 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F   ; U+0070 (p)
    defb 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78   ; U+0071 (q)
    defb 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00   ; U+0072 (r)
    defb 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00   ; U+0073 (s)
    defb 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00   ; U+0074 (t)
    defb 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00   ; U+0075 (u)
    defb 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00   ; U+0076 (v)
    defb 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00   ; U+0077 (w)
    defb 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00   ; U+0078 (x)
    defb 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F   ; U+0079 (y)
    defb 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00   ; U+007A (z)
    defb 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00   ; U+007B (defb)
    defb 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00   ; U+007C (|)
    defb 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00   ; U+007D (})
    defb 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   ; U+007E (~)
    defb 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    ; U+007F
fontend:

