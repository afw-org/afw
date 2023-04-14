// See the 'COPYING' file in the project root for licensing information.
const sizeMap = {
    "0":    "6px",
    "1":    "8px",
    "2":    "10px",
    "3":    "12px",
    "4":    "14px",
    "5":    "16px",
    "6":    "18px",
    "7":    "24px",
    "8":    "32px",
    "9":    "48px",
    "10":   "64px",    
};

export const Typography = ({ tag, text, size = "5" }) => {
    const Tag = tag ? tag : "span";

    return (
        <Tag style={{ fontSize: sizeMap[size] }}>{ text }</Tag>
    );
};

export default Typography;
